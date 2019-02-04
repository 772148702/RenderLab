#include <CppUtil/Engine/PathTracer.h>

#include <CppUtil/Engine/Scene.h>
#include <CppUtil/Engine/SObj.h>
#include <CppUtil/Engine/RayIntersector.h>
#include <CppUtil/Engine/Ray.h>
#include <CppUtil/Engine/Material.h>
#include <CppUtil/Engine/Light.h>
#include <CppUtil/Engine/LightBase.h>
#include <CppUtil/Engine/BSDF_Diffuse.h>

#include <CppUtil/Basic/Math.h>

using namespace CppUtil::Engine;
using namespace CppUtil::Basic;
using namespace glm;
using namespace std;

PathTracer::PathTracer(Scene::Ptr scene)
	: RayTracer(scene), sampleNumForAreaLight(5), maxDepth(10) { }

vec3 PathTracer::Trace(Ray::Ptr ray, int depth) {
	rayIntersector->SetRay(ray);
	Rst closestRst;
	FindClosetSObj(scene->GetRootSObj(), ray, closestRst);
	if (!closestRst.closestSObj) {
		float t = 0.5f * (normalize(ray->GetDir()).y + 1.0f);
		vec3 white(1.0f, 1.0f, 1.0f);
		vec3 blue(0.5f, 0.7f, 1.0f);
		return t * white + (1 - t)*blue;
		// return vec3(0);
	}

	// �������
	auto material = closestRst.closestSObj->GetComponent<Material>();
	if (material == nullptr)
		return vec3(1, 0, 1);

	// ��ֻ֧�� BSDF
	BSDF_Diffuse::Ptr bsdf = material->GetMat();
	if (bsdf == nullptr)
		return vec3(1, 0, 1);

	vec3 emitL = bsdf->GetEmission();

	const vec3 hitPos = ray->At(ray->GetTMax());

	auto const surfaceToObject = Math::GenCoordSpace(closestRst.n);
	auto const objectToSurface = transpose(surfaceToObject);

	// w_out ���ڱ�������ϵ
	auto w_out = normalize(objectToSurface * (-ray->GetDir()));

	vec3 sumLightL(0);
	
	// ������ƹ���ص�����
	vector<LightBase::Ptr> lights;
	vector<mat3> objectToLightVec;// ֻ��Ҫ��ת��������ʹ�� mat3
	vector<mat3> lightToObjectVec;// ֻ��Ҫ��ת��������ʹ�� mat3
	vector<mat3> lightToWorldVec;// ֻ��Ҫ��ת��������ʹ�� mat3
	vector<vec3> posInLightSpaceVec;

	auto objectToWorld = closestRst.closestSObj->GetLocalToWorldMatrix();
	auto worldToObject = inverse(objectToWorld);
	vec4 hitPos4 = vec4(hitPos, 1);

	for (auto lightComponent : scene->GetLights()) {
		mat4 lightToWorld = lightComponent->GetLightToWorldMatrixWithoutScale();
		mat4 worldToLight = inverse(lightToWorld);
		mat4 objectToLight = worldToLight * objectToWorld;

		lightToWorldVec.push_back(lightToWorld);
		lights.push_back(lightComponent->GetLight());
		objectToLightVec.push_back(objectToLight);
		lightToObjectVec.push_back(worldToObject * lightToWorld);
		posInLightSpaceVec.push_back(objectToLight * hitPos4);
	}
	int lightNum = lights.size();
	
	if (!bsdf->IsDelta()) {
		const vec3 hitPosInWorld = objectToWorld * vec4(hitPos, 1);

		vec3 dir_ToLight; // �ѱ�������������ٳ�ʼ������
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
				// ���Ը�������� PD <= 0
				if (PD <= 0)
					continue;

				const vec3 dirInObject = lightToObjectVec[i] * dir_ToLight;
				// w_in ���ڱ�������ϵ��Ӧ���ǵ�λ����
				const vec3 w_in = objectToSurface * dirInObject;

				// ������Ҫ�Բ��� Multiple Importance Sampling (MIS)
				float sumPD = bsdf->PDF(w_out, w_in) + sampleNum * PD;
				for (int k = 0; k < lightNum;k++) {
					if (k != i) {
						int sampleNum = lights[k]->IsDelta() ? 1 : sampleNumForAreaLight;
						vec3 dir = objectToLightVec[k] * dirInObject;
						sumPD += sampleNum * lights[k]->PDF(posInLightSpaceVec[i], dir_ToLight);
					}
				}

				// ����ײ���������ϵ���� dot(n, w_in) �ܼ򵥣���Ϊ n = (0, 0, 1)
				const float cos_theta = w_in.z;

				// evaluate surface bsdf
				const vec3 f = bsdf->F(w_out, w_in);

				// dirInWorld Ӧ���ǵ�λ����
				const vec3 dirInWorld = lightToWorldVec[i] * dir_ToLight;
				vec3 originInWorld = hitPosInWorld + Math::EPSILON * dirInWorld;
				// shadowRay ������������
				Ray::Ptr shadowRay = ToPtr(new Ray(originInWorld, dirInWorld));
				shadowRay->SetTMax(dist_ToLight - Math::EPSILON);
				Rst shadowRst;
				// Ӧ��ʹ��һ���Ż����ĺ���
				// ���� ray �� tMax��Ȼ��ֻҪ�ҵ�һ����ײ�󼴿ɷ��أ������ҵ������
				FindClosetSObj(scene->GetRootSObj(), shadowRay, shadowRst);
				if (!shadowRst.closestSObj)
					sumLightL += (cos_theta / sumPD) * f * lightL;
			}
		}
	}

	// ��ȣ�һ�����ʶ���
	float depthP = depth > maxDepth ? 0.5f : 1.0f;
	if (Math::Rand_F() <= depthP)
		return emitL + sumLightL;

	// ���ַ��̣�һ�����ʶ���
	vec3 mat_w_in;
	float matPD;
	// matPD һ������ 0
	const vec3 matF = bsdf->Sample_f(w_out, mat_w_in, matPD);

	float terminateProbability = 0.f;
	// Pareto principle : 2-8 principle
	// 0.2 * cos(PI / 2 * 0.8) == 0.0618
	if (!bsdf->IsDelta() && Math::Illum(matF) * abs(mat_w_in.z) < 0.0618f)
		terminateProbability = 0.8f;

	if (Math::Rand_F() < terminateProbability)
		return emitL + sumLightL;

	float sumPD = matPD;
	vec3 matRayDir = surfaceToObject * mat_w_in;
	if (bsdf->IsDelta()) {
		for (int i = 0; i < lightNum; i++) {
			int sampleNum = lights[i]->IsDelta() ? 1 : sampleNumForAreaLight;
			vec3 dir = objectToLightVec[i] * matRayDir;
			sumPD += sampleNum * lights[i]->PDF(posInLightSpaceVec[i], dir);
		}
	}

	float cosTheta = mat_w_in.z;

	Ray::Ptr matRay = ToPtr(new Ray(hitPos + Math::EPSILON * matRayDir, matRayDir));
	const vec3 matRayColor = Trace(matRay, depth + 1);

	vec3 matL = abs(cosTheta) / (sumPD*(1.f - terminateProbability)*depthP) * matF * matRayColor;

	return emitL + sumLightL + matL;
}
