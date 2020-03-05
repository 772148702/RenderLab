#ifndef _ENGINE_LIGHT_AREA_LIGHT_H_
#define _ENGINE_LIGHT_AREA_LIGHT_H_

#include <Engine/Light.h>
#include <Basic/UniformGridSampler2D.h>

namespace CppUtil {
	namespace Engine {
		// ����
		class AreaLight : public Light {
		public:
			AreaLight(const Ubpa::rgbf &color = Ubpa::rgbf(1.f), float intensity = 1.f, float width = 1.f, float height = 1.f)
				: color(color), intensity(intensity), width(width), height(height) { }

		public:
			static const Basic::Ptr<AreaLight> New(const Ubpa::rgbf &color = Ubpa::rgbf(1.f), float intensity = 1.f, float width = 1.f, float height = 1.f) {
				return Basic::New<AreaLight>(color, intensity, width, height);
			}

		public:
			float Area() const { return width * height; }
			const Ubpa::rgbf LuminancePower() const { return intensity * color; }
			const Ubpa::rgbf Luminance() const { return LuminancePower() / (Area() * Ubpa::PI<float>); }

		public:
			// ���� L ����
			// !!! p��wi ���ڹ�Դ������ռ���
			// @arg0  in���� p �������� distToLight �� PD
			// @arg1 out��wi ָ���Դ��Ϊ��λ����
			// @arg2 out��p �㵽��Դ������ľ���
			// @arg3 out�������ܶ� probability density
			virtual const Ubpa::rgbf Sample_L(const Ubpa::pointf3 & p, Ubpa::normalf & wi, float & distToLight, float & PD) const override;

			// �����ܶȺ���
			// !!! p��wi ���ڵƵ�����ռ���
			virtual float PDF(const Ubpa::pointf3 & p, const Ubpa::normalf & wi) const override;

			virtual bool IsDelta() const override { return false; }

		public:
			Ubpa::rgbf color;
			float intensity;
			float width;
			float height;

		private:
			// dirToLight ������ ��λ����
			bool Hit(const Ubpa::pointf3 & p, const Ubpa::vecf3 & dirToLight, Ubpa::pointf3 & hitPos) const;

			Basic::UniformGridSampler2D sampler;
		};
	}
}

#endif//!_ENGINE_LIGHT_AREA_LIGHT_H_
