#ifndef _CPPUTIL_ENGINE_POINT_LIGHT_H_
#define _CPPUTIL_ENGINE_POINT_LIGHT_H_

#include <CppUtil/Engine/Light.h>

namespace CppUtil {
	namespace Engine {
		class PointLight : public Light {
		public:
			PointLight(const RGBf &color = 1, float intensity = 1.0f, float radius = 10.f)
				: color(color), intensity(intensity), radius(radius) { }

		public:
			static const Basic::Ptr<PointLight> New(const RGBf &color = RGBf(1), float intensity = 1.0f, float radius = 10.f) {
				return Basic::New<PointLight>(color, intensity, radius);
			}

		protected:
			virtual ~PointLight() = default;

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

		private:
			static float Fwin(float d, float radius);

		public:
			RGBf color;
			float intensity;
			float radius;
		};
	}
}

#endif//!_CPPUTIL_ENGINE_POINT_LIGHT_H_
