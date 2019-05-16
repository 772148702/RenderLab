#ifndef _ENGINE_LIGHT_AREA_LIGHT_H_
#define _ENGINE_LIGHT_AREA_LIGHT_H_

#include <CppUtil/Engine/Light.h>
#include <CppUtil/Basic/UniformGridSampler2D.h>

namespace CppUtil {
	namespace Engine {
		// �ֲ�����ϵ�У����� -y
		class AreaLight : public Light {
		public:
			AreaLight(const RGBf &color = RGBf(1), float intensity = 1, float width = 1, float height = 1)
				: color(color), intensity(intensity), width(width), height(height) { }

		public:
			static const Basic::Ptr<AreaLight> New(const RGBf &color = RGBf(1), float intensity = 1, float width = 1, float height = 1) {
				return Basic::New<AreaLight>(color, intensity, width, height);
			}

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
			virtual const RGBf GetL(const Point3 & p, const Vec3 & dirToLight, float & distToLight) override;

			// ��ȡ L
			// ��� p �����ܱ����䵽���򷵻� vec3(0)
			virtual const RGBf GetMaxL(const Point3 & p) const override;

			// ֱ�ӷ���������
			virtual const RGBf GetMaxL() const override { return color * intensity; }

			// �����ܶȺ���
			// !!! p��wi ���ڵƵ�����ռ���
			virtual float PDF(const Point3 & p, const Normalf & wi) const override;

			virtual bool IsDelta() const override { return false; }

		public:
			RGBf color;
			float intensity;
			float width;
			float height;

		private:
			// dirToLight ������ ��λ����
			bool Hit(const Point3 & p, const Vec3 & dirToLight, Point3 & hitPos) const;

			Basic::UniformGridSampler2D sampler;
		};
	}
}

#endif//!_ENGINE_LIGHT_AREA_LIGHT_H_
