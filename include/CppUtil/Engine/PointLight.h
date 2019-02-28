#ifndef _CPPUTIL_ENGINE_POINT_LIGHT_H_
#define _CPPUTIL_ENGINE_POINT_LIGHT_H_

#include <CppUtil/Engine/LightBase.h>

namespace CppUtil {
	namespace Engine {
		class PointLight : public LightBase {
			ELE_SETUP(PointLight)
		public:
			PointLight(const glm::vec3 &color = glm::vec3(1), float intensity = 1.0f, float linear = 0.7f, float quadratic = 1.8f)
				: color(color), intensity(intensity), linear(linear), quadratic(quadratic) { }

			// ���� L ����
			// !!! p��wi ���ڵƵ�����ռ���
			// @arg0  in���� p �������� distToLight �� PD
			// @arg1 out��wi ���Ϊ�ƣ�Ϊ��λ����
			// @arg2 out��p �㵽�ƹ������ľ���
			// @arg3 out�������ܶ� probability density
			virtual glm::vec3 Sample_L(const glm::vec3& p, glm::vec3 & wi, float & distToLight, float & PD) const;

			// ��ȡ L
			// ������ܻ��й�Դ���򷵻� vec3(0)
			virtual glm::vec3 GetL(const glm::vec3& p, const glm::vec3 & dirToLight, float & distToLight) { return glm::vec3(0); };

			// ��ȡ Max L
			// ��� p �����ܱ����䵽���򷵻� vec3(0)
			// ���ڵ��Դ��˵
			virtual glm::vec3 GetMaxL(const glm::vec3 & p) const { return intensity * color; }

			// �����ܶȺ���
			// !!! p��wi ���ڵƵ�����ռ���
			virtual float PDF(const glm::vec3& p, const glm::vec3& wi) const { return 1.0f; };

			virtual bool IsDelta() const { return true; }

		public:
			glm::vec3 color;
			float intensity;

			/*
				����		������	һ����	������
				7		1.0		0.7		1.8
				13		1.0		0.35	0.44
				20		1.0		0.22	0.20
				32		1.0		0.14	0.07
				50		1.0		0.09	0.032
				65		1.0		0.07	0.017
				100		1.0		0.045	0.0075
				160		1.0		0.027	0.0028
				200		1.0		0.022	0.0019
				325		1.0		0.014	0.0007
				600		1.0		0.007	0.0002
				3250	1.0		0.0014	0.000007
			*/

			float linear;
			float quadratic;
		};
	}
}

#endif//!_CPPUTIL_ENGINE_POINT_LIGHT_H_
