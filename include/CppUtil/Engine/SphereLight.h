#include <CppUtil/Engine/Light.h>

namespace CppUtil {
	namespace Engine {
		class SphereLight : public Light {
		public:
			SphereLight(const RGBf & color = 1, float intensity = 1.f, float radius = 1.f)
				: color(color), intensity(intensity), radius(radius) { }

		public:
			static Basic::Ptr<SphereLight> New(const RGBf & color = 1, float intensity = 1.f, float radius = 1.f) {
				return Basic::New<SphereLight>(color, intensity, radius);
			}

		protected:
			virtual ~SphereLight() = default;

		public:
			float Area() const { return 4 * Basic::Math::PI * radius * radius; }
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
		};
	}
}