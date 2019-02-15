#include <CppUtil/Engine/PathTracer.h>

#include <CppUtil/Engine/BVHNode.h>

#include <CppUtil/Engine/Scene.h>
#include <CppUtil/Engine/SObj.h>
#include <CppUtil/Engine/RayIntersector.h>
#include <CppUtil/Engine/VisibilityChecker.h>
#include <CppUtil/Engine/Ray.h>
#include <CppUtil/Engine/Material.h>
#include <CppUtil/Engine/Light.h>
#include <CppUtil/Engine/LightBase.h>
#include <CppUtil/Engine/BSDF.h>

#include <CppUtil/Engine/Geometry.h>
#include <CppUtil/Engine/Sphere.h>
#include <CppUtil/Engine/Plane.h>
#include <CppUtil/Engine/TriMesh.h>

#include <CppUtil/Basic/Math.h>
#include <glm/geometric.hpp>

using namespace CppUtil::Engine;
using namespace CppUtil::Basic;
using namespace glm;
using namespace std;

// ------------ BVHInitVisitor ------------

class PathTracer::BVHInitVisitor : public EleVisitor {
	ELEVISITOR_SETUP(BVHInitVisitor)
public:
	BVHInitVisitor(PathTracer * ptx)
		: ptx(ptx) {
		Reg<Geometry>();
		Reg<Sphere>();
		Reg<Plane>();
		Reg<TriMesh>();
	}
private:
	void Visit(Geometry::Ptr geo) {
		auto primitive = geo->GetPrimitive();
		if (!primitive)
			return;

		auto w2l = geo->GetSObj()->GetWorldToLocalMatrix();
		auto l2w = inverse(w2l);
		ptx->worldToLocalMatrixes[primitive] = w2l;
		ptx->localToWorldMatrixes[primitive] = l2w;
		ptx->norm_localToWorld[geo->GetSObj()] = transpose(inverse(mat3(l2w)));

		ptx->primitive2sobj[geo->GetPrimitive()] = geo->GetSObj();
		geo->GetPrimitive()->Accept(This());
	}

	void Visit(Sphere::Ptr sphere) {
		auto matrix = ptx->localToWorldMatrixes[sphere];
		ptx->elements.push_back(sphere);
		ptx->ele2bbox[sphere] = sphere->GetBBox().Transform(matrix);
	}

	void Visit(Plane::Ptr plane) {
		auto matrix = ptx->localToWorldMatrixes[plane];
		ptx->elements.push_back(plane);
		ptx->ele2bbox[plane] = plane->GetBBox().Transform(matrix);
	}

	void Visit(TriMesh::Ptr mesh) {
		auto matrix = ptx->localToWorldMatrixes[mesh];
		auto triangles = mesh->GetTriangles();
		for (auto triangle : triangles) {
			ptx->elements.push_back(triangle);
			ptx->ele2bbox[triangle] = triangle->GetBBox().Transform(matrix);
		}
	}

private:
	PathTracer * ptx;
};

// ------------ GetPrimitiveVisitor ------------

class PathTracer::GetPrimitiveVisitor : public EleVisitor {
	ELEVISITOR_SETUP(GetPrimitiveVisitor)
public:
	GetPrimitiveVisitor() {
		Reg<Triangle>();
		Reg<Sphere>();
		Reg<Plane>();
		Reg<TriMesh>();
	}
private:
	void Visit(Triangle::Ptr obj) { primitive = obj->GetMesh(); }
	void Visit(Sphere::Ptr obj) { primitive = obj; }
	void Visit(Plane::Ptr obj) { primitive = obj; }
	void Visit(TriMesh::Ptr obj) { primitive = obj; }
public:
	Primitive::Ptr primitive;
};

// ------------ PathTracer ------------

PathTracer::PathTracer(Scene::Ptr scene)
	: RayTracer(scene), sampleNumForAreaLight(2), maxDepth(20) { }

void PathTracer::Init() {
	lights.clear();
	worldToLightVec.clear();
	dir_lightToWorldVec.clear();
	dir_worldToLightVec.clear();

	for (auto lightComponent : scene->GetLights()) {
		lights.push_back(lightComponent->GetLight());

		mat4 lightToWorld = lightComponent->GetLightToWorldMatrixWithoutScale();
		mat4 worldToLight = inverse(lightToWorld);

		worldToLightVec.push_back(worldToLight);
		dir_lightToWorldVec.push_back(lightToWorld);
		dir_worldToLightVec.push_back(worldToLight);
	}

	//bvh
	auto geos = scene->GetRoot()->GetComponentsInChildren<Geometry>();
	auto initVisitor = ToPtr(new BVHInitVisitor(this));
	for (auto geo : geos)
		geo->Accept(initVisitor);
	bvhRoot = ToPtr(new BVHNode<Element, PathTracer>(this, elements, 0, elements.size()));
}

const mat4 & PathTracer::GetEleW2LMat(Element::Ptr element) {
	auto visitor = ToPtr(new GetPrimitiveVisitor);
	element->Accept(visitor);
	return worldToLocalMatrixes[visitor->primitive];
}

const mat4 & PathTracer::GetEleL2WMat(Element::Ptr element) {
	auto visitor = ToPtr(new GetPrimitiveVisitor);
	element->Accept(visitor);
	return localToWorldMatrixes[visitor->primitive];
}

const SObj::Ptr PathTracer::GetSObj(Element::Ptr element) {
	auto visitor = ToPtr(new GetPrimitiveVisitor);
	element->Accept(visitor);
	return primitive2sobj[visitor->primitive];
}

vec3 PathTracer::Trace(Ray::Ptr ray, int depth) {
	auto rayIntersector = ToPtr(new RayIntersector(ray));
	bvhRoot->Accept(rayIntersector);
	auto & closestRst = rayIntersector->GetRst();
	if (!closestRst.closestSObj) {
		//return vec3(0);
		
		float t = 0.5f * (normalize(ray->GetDir()).y + 1.0f);
		vec3 white(1.0f, 1.0f, 1.0f);
		vec3 blue(0.5f, 0.7f, 1.0f);
		return t * white + (1 - t)*blue;
		
	}
	const vec3 worldNorm = normalize(norm_localToWorld[closestRst.closestSObj] * closestRst.n);
	//return abs(worldNorm);

	// �������
	auto material = closestRst.closestSObj->GetComponent<Material>();
	if (material == nullptr)
		return vec3(1, 0, 1);

	// ��ֻ֧�� BSDF
	BSDF::Ptr bsdf = material->GetMat();
	if (bsdf == nullptr)
		return vec3(1, 0, 1);

	vec3 emitL = bsdf->GetEmission();

	const vec3 hitPos = ray->At(ray->GetTMax());

	auto const surfaceToWorld = Math::GenCoordSpace(worldNorm);
	auto const worldToSurface = transpose(surfaceToWorld);

	// w_out ���ڱ�������ϵ������
	auto w_out = normalize(worldToSurface * (-ray->GetDir()));

	vec3 sumLightL(0);
	
	// ������ײ���ڵƹ�ռ��λ��
	vector<vec3> posInLightSpaceVec;

	vec4 hitPos4 = vec4(hitPos, 1);
	int lightNum = lights.size();
	for (int i = 0; i < lightNum; i++)
		posInLightSpaceVec.push_back(worldToLightVec[i] * hitPos4);
	
	if (!bsdf->IsDelta()) {
		vec3 dir_ToLight; // �ѱ�������������ٳ�ʼ������
		const vec3 shadowOrigin = hitPos;

		for (int i = 0; i < lightNum; i++) {
			auto const light = lights[i];

			const int sampleNum = light->IsDelta() ? 1 : sampleNumForAreaLight;

			for (int j = 0; j < sampleNum; j++) {
				float dist_ToLight;
				float PD;// �����ܶ�
				// dir_ToLight �ǵ�λ����
				const vec3 lightL = light->Sample_L(posInLightSpaceVec[i], dir_ToLight, dist_ToLight, PD);

				// �� MIT 15-462 ��Ϊ if (w_in.z < 0) continue;
				// ���ɾ����� BSDF ��Ϊ Delta ������£���Ӧ�ó��� w_in �������±ߵ����
				// ���ÿ�����Щ���ʻ��������������������
				// ����Ŀǰ��Ϊ��������� PD <= 0
				if (PD <= 0)
					continue;

				// dirInWorld Ӧ���ǵ�λ����
				const vec3 dirInWorld = dir_lightToWorldVec[i] * dir_ToLight;
				// w_in ���ڱ�������ϵ��Ӧ���ǵ�λ����
				const vec3 w_in = normalize(worldToSurface * dirInWorld);

				// ������Ҫ�Բ��� Multiple Importance Sampling (MIS)
				float sumPD = bsdf->PDF(w_out, w_in) + sampleNum * PD;
				for (int k = 0; k < lightNum; k++) {
					if (k != i) {
						int sampleNum = lights[k]->IsDelta() ? 1 : sampleNumForAreaLight;
						vec3 dirInLight = dir_worldToLightVec[k] * dirInWorld;
						sumPD += sampleNum * lights[k]->PDF(posInLightSpaceVec[i], dirInLight);
					}
				}

				// ����ײ���������ϵ���� dot(n, w_in) �ܼ򵥣���Ϊ n = (0, 0, 1)
				const float abs_cos_theta = abs(w_in.z);

				// evaluate surface bsdf
				const vec3 f = bsdf->F(w_out, w_in);

				// shadowRay ������������
				Ray::Ptr shadowRay = ToPtr(new Ray(shadowOrigin, dirInWorld));
				float tMax = dist_ToLight / length(dirInWorld) - 0.001f;
				auto checker = ToPtr(new VisibilityChecker(shadowRay, tMax));
				bvhRoot->Accept(checker);
				auto shadowRst = checker->GetRst();
				if (!shadowRst.IsIntersect())
					sumLightL += (abs_cos_theta / sumPD) * f * lightL;
			}
		}
	}
	// ��ȣ���������
	float depthP = depth < maxDepth ? 0.f : 0.5f;
	if (Math::Rand_F() < depthP)
		return emitL + sumLightL;

	// ���� BSDF
	vec3 mat_w_in;
	float matPD;
	const vec3 matF = bsdf->Sample_f(w_out, mat_w_in, matPD);
	const vec3 matRayDirInWorld = surfaceToWorld * mat_w_in;
	const float abs_cosTheta = abs(mat_w_in.z);

	if (matPD <= 0)
		return emitL + sumLightL;

	// һ�����ʶ���
	float terminateProbability = 0.f;
	// ����һ��Ҫ�� illumination * cos(theta)
	if (!bsdf->IsDelta() && Math::Illum(matF) * abs_cosTheta < 0.0618f)
		terminateProbability = 0.8f;

	if (Math::Rand_F() < terminateProbability)
		return emitL + sumLightL;

	// ��Ҫ�Բ���
	float sumPD = matPD;
	if (!bsdf->IsDelta()) {
		for (int i = 0; i < lightNum; i++) {
			int sampleNum = lights[i]->IsDelta() ? 1 : sampleNumForAreaLight;
			vec3 dirInLight = dir_worldToLightVec[i] * matRayDirInWorld;
			sumPD += sampleNum * lights[i]->PDF(posInLightSpaceVec[i], dirInLight);
		}
	}

	Ray::Ptr matRay = ToPtr(new Ray(hitPos, matRayDirInWorld));
	const vec3 matRayColor = Trace(matRay, depth + 1);

	vec3 matL = abs_cosTheta / (sumPD * (1.f - terminateProbability) * (1.f - depthP)) * matF * matRayColor;

	return emitL + sumLightL + matL;
}
