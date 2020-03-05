#include "PLDM_Generator.h"

#include <Qt/RawAPI_Define.h>
#include <Qt/RawAPI_OGLW.h>

#include <Engine/Scene.h>
#include <Engine/SObj.h>

#include <Engine/CmptTransform.h>

#include <Engine/CmptLight.h>
#include <Engine/PointLight.h>

#include <Engine/CmptGeometry.h>
#include <Engine/Sphere.h>
#include <Engine/Plane.h>
#include <Engine/TriMesh.h>
#include <Engine/Disk.h>
#include <Engine/Capsule.h>

#include <ROOT_PATH.h>

using namespace CppUtil;
using namespace CppUtil::QT;
using namespace CppUtil::Engine;
using namespace CppUtil::OpenGL;
using namespace CppUtil::Basic;
using namespace std;

const string rootPath = ROOT_PATH;

namespace CppUtil {
	namespace Engine {
		const string str_PointLight_prefix = "data/shaders/Engine/PointLight/";
		const string str_genDepth = "genDepth";
		const string str_genDepth_vs = str_PointLight_prefix + str_genDepth + ".vs";
		const string str_genDepth_gs = str_PointLight_prefix + str_genDepth + "[1]s";
		const string str_genDepth_fs = str_PointLight_prefix + str_genDepth + ".fs";
	}
}

PLDM_Generator::PLDM_Generator(QT::RawAPI_OGLW * pOGLW, float lightNear, float lightFar)
	: pOGLW(pOGLW), depthMapSize(1024), lightNear(lightNear), lightFar(lightFar)
{
	RegMemberFunc<Scene>(&PLDM_Generator::Visit);
	RegMemberFunc<SObj>(&PLDM_Generator::Visit);

	RegMemberFunc<Sphere>(&PLDM_Generator::Visit);
	RegMemberFunc<Plane>(&PLDM_Generator::Visit);
	RegMemberFunc<TriMesh>(&PLDM_Generator::Visit);
	RegMemberFunc<Disk>(&PLDM_Generator::Visit);
	RegMemberFunc<Capsule>(&PLDM_Generator::Visit);
}

void PLDM_Generator::Init() {
	shader_genDepth = Shader(ROOT_PATH + str_genDepth_vs, ROOT_PATH + str_genDepth_fs, ROOT_PATH + str_genDepth_gs);
	shader_genDepth.UniformBlockBind("Camera", 0);
	shader_genDepth.SetFloat("lightFar", lightFar);
}

void PLDM_Generator::Visit(Ptr<Scene> scene) {
	if (!scene || !scene->GetRoot()) {
		printf("ERROR::PLDM_Generator::Visit(Ptr<Scene> scene):\n"
			"\t""scene or scene's root is nullptr\n");
		return;
	}

	scene->SetWriteLock(true);

	GLint origFBO;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &origFBO);
	GLint origViewport[4];
	glGetIntegerv(GL_VIEWPORT, origViewport);

	modelVec.clear();
	modelVec.push_back(Ubpa::transformf::eye());

	// regist
	for (auto cmptLight : scene->GetCmptLights()) {
		if (lightMap.find(cmptLight) != lightMap.end())
			continue;

		auto pointLight = CastTo<PointLight>(cmptLight->light);
		if (!pointLight)
			continue;

		FBO FBO_DepthMap(depthMapSize, depthMapSize, FBO::ENUM_TYPE_CUBE_DEPTH);
		Texture depthMap(FBO_DepthMap.GetDepthTexture().GetID(), Texture::ENUM_TYPE_CUBE_MAP);
		lightMap[cmptLight] = FBO_Tex(FBO_DepthMap, depthMap);
	}

	auto shadowProj = Ubpa::transformf::perspective(Ubpa::to_radian(90.f), 1.f, lightNear, lightFar);
	auto nextIt = lightMap.begin();
	while (nextIt != lightMap.end()) {
		auto curIt = nextIt;
		nextIt++;

		if (curIt->first.expired()) {
			lightMap.erase(curIt);
			continue;
		}
		auto lightComponent = curIt->first.lock();

		curIt->second.fbo.Use();
		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, depthMapSize, depthMapSize);
		glClear(GL_DEPTH_BUFFER_BIT);

		auto lightPos = lightComponent->GetSObj()->GetWorldPos();
		shader_genDepth.SetVecf3("lightPos", lightPos.cast_to<Ubpa::valf3>());

		std::vector<Ubpa::transformf> shadowTransforms;
		shadowTransforms.push_back(shadowProj *
			Ubpa::transformf::look_at(lightPos, lightPos + Ubpa::vecf3(1.0, 0.0, 0.0), Ubpa::vecf3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *
			Ubpa::transformf::look_at(lightPos, lightPos + Ubpa::vecf3(-1.0, 0.0, 0.0), Ubpa::vecf3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *
			Ubpa::transformf::look_at(lightPos, lightPos + Ubpa::vecf3(0.0, 1.0, 0.0), Ubpa::vecf3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(shadowProj *
			Ubpa::transformf::look_at(lightPos, lightPos + Ubpa::vecf3(0.0, -1.0, 0.0), Ubpa::vecf3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(shadowProj *
			Ubpa::transformf::look_at(lightPos, lightPos + Ubpa::vecf3(0.0, 0.0, 1.0), Ubpa::vecf3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *
			Ubpa::transformf::look_at(lightPos, lightPos + Ubpa::vecf3(0.0, 0.0, -1.0), Ubpa::vecf3(0.0, -1.0, 0.0)));
		for (size_t i = 0; i < 6; ++i)
			shader_genDepth.SetMatf4("shadowMatrices[" + to_string(i) + "]", shadowTransforms[i].data());

		scene->GetRoot()->Accept(This());
	}

	glBindFramebuffer(GL_FRAMEBUFFER, origFBO);
	glViewport(origViewport[0], origViewport[1], origViewport[2], origViewport[3]);

	scene->SetWriteLock(false);
}

void PLDM_Generator::Visit(Ptr<SObj> sobj) {
	auto geometry = sobj->GetComponent<CmptGeometry>();
	auto children = sobj->GetChildren();
	// ��������²���Ҫ transform
	if ((geometry == nullptr || !geometry->primitive) && children.size() == 0)
		return;

	auto cmptTransform = sobj->GetComponent<CmptTransform>();
	if (cmptTransform != nullptr)
		modelVec.push_back(modelVec.back() * cmptTransform->GetTransform());

	if (geometry && geometry->primitive)
		geometry->primitive->Accept(This());

	for (auto child : children)
		child->Accept(This());

	if (cmptTransform != nullptr)
		modelVec.pop_back();
}


void PLDM_Generator::Visit(Ptr<Sphere> sphere) {
	shader_genDepth.SetMatf4("model", modelVec.back().data());
	shader_genDepth.SetBool("isOffset", false);
	pOGLW->GetVAO(ShapeType::Sphere).Draw(shader_genDepth);
}

void PLDM_Generator::Visit(Ptr<Plane> plane) {
	shader_genDepth.SetMatf4("model", modelVec.back().data());
	shader_genDepth.SetBool("isOffset", false);
	pOGLW->GetVAO(ShapeType::Plane).Draw(shader_genDepth);
}

void PLDM_Generator::Visit(Ptr<TriMesh> mesh) {
	shader_genDepth.SetMatf4("model", modelVec.back().data());
	shader_genDepth.SetBool("isOffset", false);
	pOGLW->GetVAO(mesh).Draw(shader_genDepth);
}

void PLDM_Generator::Visit(Ptr<Disk> disk) {
	shader_genDepth.SetMatf4("model", modelVec.back().data());
	shader_genDepth.SetBool("isOffset", false);
	pOGLW->GetVAO(ShapeType::Disk).Draw(shader_genDepth);
}

void PLDM_Generator::Visit(Ptr<Capsule> capsule) {
	shader_genDepth.SetMatf4("model", modelVec.back().data());
	shader_genDepth.SetBool("isOffset", true);
	shader_genDepth.SetFloat("offset", capsule->height / 2 - 1);
	pOGLW->GetVAO(ShapeType::Capsule).Draw(shader_genDepth);
}

const Texture PLDM_Generator::GetDepthCubeMap(PtrC<CmptLight> light) const {
	auto target = lightMap.find(light);
	if (target == lightMap.end())
		return Texture::InValid;

	return target->second.tex;
}
