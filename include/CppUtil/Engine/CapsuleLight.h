#ifndef _CPPUTIL_ENGINE_LIGHT_CAPSULE_LIGHT_H_
#define _CPPUTIL_ENGINE_LIGHT_CAPSULE_LIGHT_H_

#include <CppUtil/Engine/Light.h>

namespace CppUtil {
	namespace Engine {
		class CapsuleLight : public Light {
		public:
			CapsuleLight(const RGBf & color = 1.f, float intensity = 1.f, float radius = 1.f, float height = 2.f)
				: color(color), intensity(intensity), radius(radius), height(height) { }

		public:
			static const Basic::Ptr<CapsuleLight> New(const RGBf & color = 1.f, float intensity = 1.f, float radius = 1.f, float height = 2.f) {
				return Basic::New<CapsuleLight>(color, intensity, radius, height);
			}

		public:
			virtual ~CapsuleLight() = default;

		public:
			float Area() const { return 4 * Basic::Math::PI * radius * radius + 2 * Basic::Math::PI * radius * height; }
			const RGBf LuminancePower() const { return intensity * color; }
			const RGBf Luminance() const { return LuminancePower() / (Area() * Basic::Math::PI); }

		public:
			// ���� L ����
			// !!! p��wi ���ڹ�Դ������ռ���
			// @arg0  in���� p �������� distToLight �� PD
			// @arg1 out��wi ָ���Դ��Ϊ��λ����
			// @arg2 out��p �㵽��Դ������ľ���
			// @arg3 out�������ܶ� probability density
			virtual const RGBf Sample_L(const Point3 & p, Normalf & wi, float & distToLight, float & PD) const override;

			// �����ܶȺ���
			// !!! p��wi ���ڵƵ�����ռ���
			virtual float PDF(const Point3 & p, const Normalf & wi) const override;

			virtual bool IsDelta() const override { return false; }

		public:
			RGBf color;
			float intensity;
			float radius;
			float height;
		};
	}
}


#endif // !_CPPUTIL_ENGINE_LIGHT_CAPSULE_LIGHT_H_
