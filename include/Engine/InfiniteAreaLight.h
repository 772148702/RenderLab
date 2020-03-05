#pragma once

#include <Engine/Light.h>
#include <Basic/AliasMethod.h>

namespace CppUtil {
	namespace Basic {
		class Image;
	}

	namespace Engine {
		class InfiniteAreaLight : public Light {
		public:
			InfiniteAreaLight(Basic::Ptr<Basic::Image> img, const Ubpa::rgbf & colorFactor = Ubpa::rgbf(1.f), float intensity = 1.f)
				: colorFactor(colorFactor), intensity(intensity) { SetImg(img); }

		public:
			static const Basic::Ptr<InfiniteAreaLight> New(Basic::Ptr<Basic::Image> img, const Ubpa::rgbf & colorFactor = Ubpa::rgbf(1.f), float intensity = 1.f) {
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
			virtual const Ubpa::rgbf Sample_L(const Ubpa::pointf3 & p, Ubpa::normalf & wi, float & distToLight, float & PD) const override;

			// �����ܶȺ���
			// !!! p��wi ���ڵƵ�����ռ���
			virtual float PDF(const Ubpa::pointf3 & p, const Ubpa::normalf & wi) const override;

			virtual bool IsDelta() const override { return false; }

			// ����Щû�л����κ���������ߵ���
			virtual const Ubpa::rgbf Le(const ERay & ray) const override;

		private:
			const Ubpa::rgbf GetColor(const Ubpa::pointf2 & texcoord) const;

		public:
			Ubpa::rgbf colorFactor;
			float intensity;

		private:
			Basic::Ptr<Basic::Image> img;
			Basic::AliasMethod aliasMethod;
		};
	}
}
