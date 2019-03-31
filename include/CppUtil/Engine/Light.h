#ifndef _ENGINE_LIGHT_LIGHT_H_
#define _ENGINE_LIGHT_LIGHT_H_

#include <CppUtil/Basic/Element.h>

#include <glm/glm.hpp>

namespace CppUtil {
	namespace Engine {
		class Light : public Basic::Element {
			ELE_SETUP(Light)
		public:

			// ���� L ����
			// !!! p��wi ���ڵƵ�����ռ���
			// @arg0  in���� p �������� distToLight �� PD
			// @arg1 out��wi ���Ϊ�ƣ�Ϊ��λ����
			// @arg2 out��p �㵽�ƹ������ľ���
			// @arg3 out�������ܶ� probability density
			virtual glm::vec3 Sample_L(const glm::vec3& p, glm::vec3 & wi, float & distToLight, float & PD) const = 0;

			// ��ȡ L
			// ������ܻ��й�Դ���򷵻� vec3(0)
			virtual glm::vec3 GetL(const glm::vec3& p, const glm::vec3 & dirToLight, float & distToLight) = 0;

			// ��ȡ L
			// ��� p �����ܱ����䵽���򷵻� vec3(0)
			virtual glm::vec3 GetMaxL(const glm::vec3 & p) const = 0;

			// ֱ�ӷ���������
			virtual glm::vec3 GetMaxL() const = 0;

			// �����ܶȺ���
			// !!! p��wi ���ڵƵ�����ռ���
			virtual float PDF(const glm::vec3& p, const glm::vec3& wi) const = 0;
			
			virtual bool IsDelta() const = 0;
		};
	}
}

#endif//!_ENGINE_LIGHT_LIGHT_H_
