#pragma once

#include <Engine/Light.h>

namespace CppUtil {
	namespace Engine {
		class CapsuleLight : public Light {
		public:
			CapsuleLight(const Ubpa::rgbf & color = 1.f, float intensity = 1.f, float radius = 1.f, float height = 2.f)
				: color(color), intensity(intensity), radius(radius), height(height) { }

		public:
			static const Basic::Ptr<CapsuleLight> New(const Ubpa::rgbf & color = 1.f, float intensity = 1.f, float radius = 1.f, float height = 2.f) {
				return Basic::New<CapsuleLight>(color, intensity, radius, height);
			}

		public:
			virtual ~CapsuleLight() = default;

		public:
			float Area() const { return 4 * Ubpa::PI<float> * radius * radius + 2 * Ubpa::PI<float> * radius * height; }
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
			float radius;
			float height;
		};
	}
}
