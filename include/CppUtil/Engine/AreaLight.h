#ifndef _ENGINE_LIGHT_AREA_LIGHT_H_
#define _ENGINE_LIGHT_AREA_LIGHT_H_

#include <CppUtil/Engine/Light.h>
#include <CppUtil/Basic/UniformGridSampler2D.h>

namespace CppUtil {
	namespace Engine {
		class AreaLight : public Light {
			ELE_SETUP(AreaLight)
		public:
			AreaLight(const RGBf &color = RGBf(1), float intensity = 1, float width = 1, float height = 1)
				: color(color), intensity(intensity), width(width), height(height) { }

			// ���� L ����
			// !!! p��wi ���ڵƵ�����ռ���
			// @arg0  in���� p �������� distToLight �� PD
			// @arg1 out��wi ���Ϊ�ƣ�Ϊ��λ����
			// @arg2 out��p �㵽�ƹ������ľ���
			// @arg3 out�������ܶ� probability density
			virtual RGBf Sample_L(const Pointf & p, Normalf & wi, float & distToLight, float & PD) const;

			// ��ȡ L
			// ������ܻ��й�Դ���򷵻� 0
			virtual RGBf GetL(const Pointf & p, const Vectorf & dirToLight, float & distToLight);

			// ��ȡ L
			// ��� p �����ܱ����䵽���򷵻� vec3(0)
			virtual RGBf GetMaxL(const Pointf & p) const = 0;

			// ֱ�ӷ���������
			virtual RGBf GetMaxL() const { return color * intensity; }

			// �����ܶȺ���
			// !!! p��wi ���ڵƵ�����ռ���
			virtual float PDF(const Pointf & p, const Normalf & wi) const;

			virtual bool IsDelta() const { return false; }

		public:
			RGBf color;
			float intensity;
			float width;
			float height;

		private:
			// dirToLight ������ ��λ����
			bool Hit(const Pointf & p, const Vectorf & dirToLight, Pointf & hitPos) const;

			Basic::UniformGridSampler2D sampler;
		};
	}
}

#endif//!_ENGINE_LIGHT_AREA_LIGHT_H_
