#ifndef _ENGINE_LIGHT_LIGHT_H_
#define _ENGINE_LIGHT_LIGHT_H_

#include <Engine/Ray.h>

#include <Basic/Element.h>

#include <UGM/rgb.h>
#include <UGM/point.h>
#include <UGM/vec.h>
#include <UGM/normal.h>

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
			virtual const Ubpa::rgbf Sample_L(const Ubpa::pointf3 & p, Ubpa::normalf & wi, float & distToLight, float & PD) const = 0;

			// �����ܶȺ���
			// !!! p��wi ���ڵƵ�����ռ���
			virtual float PDF(const Ubpa::pointf3 & p, const Ubpa::normalf & wi) const = 0;

			virtual bool IsDelta() const = 0;

			// ����Щû�л����κ���������ߵ���
			virtual const Ubpa::rgbf Le(const ERay & ray) const { return Ubpa::rgbf(0.f); }
		};
	}
}

#endif//!_ENGINE_LIGHT_LIGHT_H_
