#ifndef _ENGINE_LIGHT_LIGHT_H_
#define _ENGINE_LIGHT_LIGHT_H_

#include <CppUtil/Basic/Element.h>

#include <CppUtil/Basic/UGM/RGB.h>
#include <CppUtil/Basic/UGM/Point3.h>
#include <CppUtil/Basic/UGM/Vector3.h>
#include <CppUtil/Basic/UGM/Normal.h>

namespace CppUtil {
	namespace Engine {
		class LightBase : public Basic::ElementBase {
		protected:
			LightBase() = default;
			virtual ~LightBase() = default;

		public:
			// ���� L ����
			// !!! p��wi ���ڵƵ�����ռ���
			// @arg0  in���� p �������� distToLight �� PD
			// @arg1 out��wi ���Ϊ�ƣ�Ϊ��λ����
			// @arg2 out��p �㵽�ƹ������ľ���
			// @arg3 out�������ܶ� probability density
			virtual const RGBf Sample_L(const Point3 & p, Normalf & wi, float & distToLight, float & PD) const = 0;

			// ��ȡ L
			// ������ܻ��й�Դ���򷵻� 0
			virtual const RGBf GetL(const Point3 & p, const Vec3 & dirToLight, float & distToLight) = 0;

			// ��ȡ L
			// ��� p �����ܱ����䵽���򷵻� vec3(0)
			virtual const RGBf GetMaxL(const Point3 & p) const = 0;

			// ֱ�ӷ���������
			virtual const RGBf GetMaxL() const = 0;

			// �����ܶȺ���
			// !!! p��wi ���ڵƵ�����ռ���
			virtual float PDF(const Point3 & p, const Normalf & wi) const = 0;

			virtual bool IsDelta() const = 0;
		};

		template<typename ImplT, typename BaseT = LightBase>
		class Light : public Basic::Element<ImplT, BaseT> {
		protected:
			using Element<ImplT, BaseT>::Element;
			virtual ~Light() = default;
		};
	}
}

#endif//!_ENGINE_LIGHT_LIGHT_H_
