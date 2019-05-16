#include <CppUtil/Engine/RasterBase.h>

#include "PLDM_Generator.h"

#include <CppUtil/Engine/Scene.h>
#include <CppUtil/Engine/SObj.h>

#include <CppUtil/Engine/CmptMaterial.h>

#include <CppUtil/Engine/CmptTransform.h>

#include <CppUtil/Engine/CmptGeometry.h>
#include <CppUtil/Engine/Sphere.h>
#include <CppUtil/Engine/Plane.h>
#include <CppUtil/Engine/TriMesh.h>
#include <CppUtil/Engine/AllBSDFs.h>

#include <CppUtil/Engine/CmptLight.h>
#include <CppUtil/Engine/PointLight.h>
#include <CppUtil/Engine/DirectionalLight.h>

#include <CppUtil/Qt/RawAPI_OGLW.h>
#include <CppUtil/Qt/RawAPI_Define.h>

#include <CppUtil/OpenGL/VAO.h>
#include <CppUtil/OpenGL/CommonDefine.h>
#include <CppUtil/OpenGL/Texture.h>
#include <CppUtil/OpenGL/Shader.h>
#include <CppUtil/OpenGL/Camera.h>
#include <CppUtil/OpenGL/FBO.h>

#include <CppUtil/Basic/LambdaOp.h>
#include <CppUtil/Basic/Sphere.h>
#include <CppUtil/Basic/Plane.h>
#include <CppUtil/Basic/Image.h>

#include <ROOT_PATH.h>

using namespace CppUtil::Engine;
using namespace CppUtil::QT;
using namespace CppUtil::OpenGL;
using namespace CppUtil::Basic;
using namespace CppUtil;
using namespace Define;
using namespace std;

const int RasterBase::maxPointLights = 8;
const string rootPath = ROOT_PATH;

bool RasterBase::ShaderCompare::operator()(const OpenGL::Shader & lhs, const OpenGL::Shader & rhs) const{
	if (!lhs.IsValid())
		return true;

	if (!rhs.IsValid())
		return false;

	return lhs.GetID() < rhs.GetID();
}

RasterBase::RasterBase(RawAPI_OGLW * pOGLW, Ptr<Scene> scene)
	: pOGLW(pOGLW), scene(scene), pldmGenerator(PLDM_Generator::New(pOGLW)) {
	RegMemberFunc<SObj>(&RasterBase::Visit);

	// primitive
	RegMemberFunc<Engine::Sphere>(&RasterBase::Visit);
	RegMemberFunc<Engine::Plane>(&RasterBase::Visit);
	RegMemberFunc<TriMesh>(&RasterBase::Visit);

	// bsdf
	RegMemberFunc<BSDF_Diffuse>(&RasterBase::Visit);
	RegMemberFunc<BSDF_Emission>(&RasterBase::Visit);
	RegMemberFunc<BSDF_Glass>(&RasterBase::Visit);
	RegMemberFunc<BSDF_Mirror>(&RasterBase::Visit);
	RegMemberFunc<BSDF_CookTorrance>(&RasterBase::Visit);
	RegMemberFunc<BSDF_MetalWorkflow>(&RasterBase::Visit);
	RegMemberFunc<BSDF_FrostedGlass>(&RasterBase::Visit);
}

void RasterBase::Draw() {
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	UpdateLights();

	scene->Accept(pldmGenerator);

	modelVec.clear();
	modelVec.push_back(Transform(1.f));
	scene->GetRoot()->Accept(This());
}

void RasterBase::OGL_Init() {
	glGenBuffers(1, &pointLightsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, pointLightsUBO);
	glBufferData(GL_UNIFORM_BUFFER, 400, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, pointLightsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glGenBuffers(1, &directionalLightsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, directionalLightsUBO);
	glBufferData(GL_UNIFORM_BUFFER, 272, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, directionalLightsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	pldmGenerator->OGL_Init();
}

void RasterBase::InitShaders() {
	InitShader_Basic();
}

void RasterBase::InitShader_Basic() {
	shader_basic = Shader(rootPath + str_Basic_P3_vs, rootPath + str_Basic_fs);

	RegShader(shader_basic);
}

void RasterBase::Visit(Ptr<SObj> sobj) {
	auto geometry = sobj->GetComponent<CmptGeometry>();
	auto material = sobj->GetComponent<CmptMaterial>();
	auto children = sobj->GetChildren();

	// 这种情况下不需要 transform
	if ((!geometry || !geometry->primitive || !material || !material->material)
		&& children.size() == 0)
		return;

	auto cmptTransform = sobj->GetComponent<CmptTransform>();
	if (cmptTransform != nullptr)
		modelVec.push_back(modelVec.back() * cmptTransform->GetTransform());

	if (geometry && geometry->primitive && material && material->material) {
		material->material->Accept(This());
		geometry->primitive->Accept(This());
	}

	for (auto child : children)
		child->Accept(This());

	if (cmptTransform != nullptr)
		modelVec.pop_back();
}

void RasterBase::Visit(Ptr<Engine::Sphere> sphere) {
	curShader.SetMat4f("model", modelVec.back());
	pOGLW->GetVAO(ShapeType::Sphere).Draw(curShader);
}

void RasterBase::Visit(Ptr<Engine::Plane> plane) {
	curShader.SetMat4f("model", modelVec.back());
	pOGLW->GetVAO(ShapeType::Plane).Draw(curShader);
}

void RasterBase::Visit(Ptr<TriMesh> mesh) {
	curShader.SetMat4f("model", modelVec.back());
	pOGLW->GetVAO(mesh).Draw(curShader);
}

void RasterBase::UpdateLights() {
	UpdatePointLights();
	UpdateDirectionalLights();
}

void RasterBase::UpdatePointLights() {
	pointLight2idx.clear();

	int pointLightIdx = 0;
	glBindBuffer(GL_UNIFORM_BUFFER, pointLightsUBO);
	for (auto cmptLight : scene->GetCmptLights()) {
		auto pointLight = CastTo<PointLight>(cmptLight->light);
		if (!pointLight)
			continue;

		pointLight2idx[pointLight] = pointLightIdx;

		Point3 position = cmptLight->GetSObj()->GetWorldPos();

		int base = 16 + 48 * pointLightIdx;
		glBufferSubData(GL_UNIFORM_BUFFER, base, 12, position.Data());
		auto lightL = pointLight->intensity * pointLight->color;
		glBufferSubData(GL_UNIFORM_BUFFER, base + 16, 12, lightL.Data());
		glBufferSubData(GL_UNIFORM_BUFFER, base + 28, 4, &pointLight->linear);
		glBufferSubData(GL_UNIFORM_BUFFER, base + 32, 4, &pointLight->quadratic);

		pointLightIdx++;
	}
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 4, &pointLightIdx); // 点光源个数即为 pointLightIdx
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void RasterBase::UpdateDirectionalLights() {
	directionalLight2idx.clear();

	int directionalLightIdx = 0;
	glBindBuffer(GL_UNIFORM_BUFFER, directionalLightsUBO);
	for (auto cmptLight : scene->GetCmptLights()) {
		auto directionalLight = CastTo<DirectionalLight>(cmptLight->light);
		if (!directionalLight)
			continue;

		directionalLight2idx[directionalLight] = directionalLightIdx;

		Vec3f dir = cmptLight->GetSObj()->GetLocalToWorldMatrix()(Normalf(0, -1, 0));

		int base = 16 + 32 * directionalLightIdx;
		auto lightL = directionalLight->intensity * directionalLight->color;
		glBufferSubData(GL_UNIFORM_BUFFER, base, 12, lightL.Data());
		glBufferSubData(GL_UNIFORM_BUFFER, base + 16, 12, dir.Data());

		directionalLightIdx++;
	}
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 4, &directionalLightIdx); // 方向光个数即为 directionalLightIdx
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void RasterBase::UsePointLightDepthMap(const Shader & shader) const {
	auto target = shader2depthmapBase.find(shader);
	if (target == shader2depthmapBase.cend()) {
		printf("ERROR::RasterBase::UsePointLightDepthMap:\n"
			"\t""shader not regist\n");
		return;
	}
	const auto depthmapBase = target->second;

	for (auto cmptLight : scene->GetCmptLights()) {
		auto pointLight = CastTo<PointLight>(cmptLight->light);
		auto target = pointLight2idx.find(pointLight);
		if (target == pointLight2idx.cend())
			return;
		const auto pointLightIdx = target->second;

		pldmGenerator->GetDepthCubeMap(cmptLight).Use(depthmapBase + pointLightIdx);
	}
}

void RasterBase::RegShader(const OpenGL::Shader & shader, int depthmapBase) {
	shader.UniformBlockBind("Camera", 0);

	if (depthmapBase < 0) // 无需计算光
		return;

	shader.UniformBlockBind("PointLights", 1);
	shader.UniformBlockBind("DirectionalLights", 2);

	for (int i = 0; i < maxPointLights; i++)
		shader.SetInt("pointLightDepthMap" + to_string(i), depthmapBase + i);

	shader.SetFloat("lightFar", pldmGenerator->GetLightFar());

	shader2depthmapBase[shader] = depthmapBase;
}
