#ifndef _ENGINE_LIGHT_LIGHT_H_
#define _ENGINE_LIGHT_LIGHT_H_

#include <Engine/Ray.h>

#include <Basic/Element.h>

#include <Basic/UGM/RGB.h>
#include <Basic/UGM/Point3.h>
#include <Basic/UGM/Vector3.h>
#include <Basic/UGM/Normal.h>

namespace CppUtil {
	namespace Engine {
		class Light : public Basic::Element {
		protected:
			Light() = default;
			virtual ~Light() = default;

		public:
			// ���� L ����
			// !!! p��wi ���ڹ�Դ������ռ���
			// @arg0  in���� p �������� distToLight �� PD
			// @arg1 out��wi ָ���Դ��Ϊ��λ����
			// @arg2 out��p �㵽��Դ������ľ���
			// @arg3 out�������ܶ� probability density
			virtual const RGBf Sample_L(const Point3 & p, Normalf & wi, float & distToLight, float & PD) const = 0;

			// �����ܶȺ���
			// !!! p��wi ���ڵƵ�����ռ���
			virtual float PDF(const Point3 & p, const Normalf & wi) const = 0;

			virtual bool IsDelta() const = 0;

			// ����Щû�л����κ���������ߵ���
			virtual const RGBf Le(const ERay & ray) const { return RGBf(0.f); }
		};
	}
}

#endif//!_ENGINE_LIGHT_LIGHT_H_
