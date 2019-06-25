#ifndef _CPPUTIL_ENGINE_SPOT_LIGHT_H_
#define _CPPUTIL_ENGINE_SPOT_LIGHT_H_

#include <CppUtil/Engine/Light.h>

namespace CppUtil {
	namespace Engine {
		// �� -y ��������
		class SpotLight : public Light {
		public:
			SpotLight(const RGBf &color = RGBf(1), float intensity = 1.0f, float radius = 10.f,
				float angle = 90.f, float fullRatio = 0.8f)
				: color(color), intensity(intensity), radius(radius),
				angle(angle), fullRatio(fullRatio) { }

		public:
			static const Basic::Ptr<SpotLight> New(const RGBf &color = RGBf(1), float intensity = 1.0f, float radius = 10.f,
				float angle = 90.f, float fullRatio = 0.8f) {
				return Basic::New<SpotLight>(color, intensity, radius, angle, fullRatio);
			}

		protected:
			virtual ~SpotLight() = default;

		public:
			// ���� L ����
			// !!! p��wi ���ڵƵ�����ռ���
			// @arg0  in���� p �������� distToLight �� PD
			// @arg1 out��wi ���Ϊ�ƣ�Ϊ��λ����
			// @arg2 out��p �㵽�ƹ������ľ���
			// @arg3 out�������ܶ� probability density
			virtual const RGBf Sample_L(const Point3 & p, Normalf & wi, float & distToLight, float & PD) const override;

			// ��ȡ L
			// ������ܻ��й�Դ���򷵻� 0
			virtual const RGBf GetL(const Point3 & p, const Vec3 & dirToLight, float & distToLight) override { return RGBf(0.f); }

			// ��ȡ L
			// ��� p �����ܱ����䵽���򷵻� vec3(0)
			virtual const RGBf GetMaxL(const Point3 & p) const override { return color * intensity; };

			// ֱ�ӷ���������
			virtual const RGBf GetMaxL() const override { return color * intensity; }

			// �����ܶȺ���
			// !!! p��wi ���ڵƵ�����ռ���
			virtual float PDF(const Point3 & p, const Normalf & wi) const override { return 0; }

			virtual bool IsDelta() const override { return true; }

		public:
			float CosHalfAngle() const{
				return cos(Basic::Math::Radians(angle) / 2);
			}
			float CosFalloffAngle() const {
				return cos(Basic::Math::Radians(angle) / 2 * fullRatio);
			}

		private:
			static float Fwin(float d, float radius);
			float Falloff(const Normalf & wi) const;

		public:
			RGBf color;
			float intensity;
			float radius;

			float angle; // [1 - 179]
			float fullRatio; // [0 -- 1]
		};
	}
}

#endif//!_CPPUTIL_ENGINE_SPOT_LIGHT_H_
