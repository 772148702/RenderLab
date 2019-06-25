#ifndef _CPPUTIL_ENGINE_INFINITE_AREA_LIGHT_H_
#define _CPPUTIL_ENGINE_INFINITE_AREA_LIGHT_H_

#include <CppUtil/Engine/Light.h>
#include <CppUtil/Basic/AliasMethod.h>

namespace CppUtil {
	namespace Basic {
		class Image;
	}

	namespace Engine {
		class InfiniteAreaLight : public Light {
		public:
			InfiniteAreaLight(Basic::Ptr<Basic::Image> img, const RGBf & colorFactor = RGBf(1.f), float intensity = 1.f)
				: colorFactor(colorFactor), intensity(intensity) { SetImg(img); }

		public:
			static const Basic::Ptr<InfiniteAreaLight> New(Basic::Ptr<Basic::Image> img, const RGBf & colorFactor = RGBf(1.f), float intensity = 1.f) {
				return Basic::New<InfiniteAreaLight>(img, colorFactor, intensity);
			}

		public:
			const Basic::PtrC<Basic::Image> GetImg() const { return img; }
			void SetImg(Basic::Ptr<Basic::Image> img);

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

			// ����Щû�л����κ���������ߵ���
			virtual const RGBf Le(const ERay & ray) const override;

		private:
			const RGBf GetColor(const Point2 & texcoord) const;

		public:
			RGBf colorFactor;
			float intensity;

		private:
			Basic::Ptr<Basic::Image> img;
			Basic::AliasMethod aliasMethod;
		};
	}
}

#endif // !_CPPUTIL_ENGINE_INFINITE_AREA_LIGHT_H_
