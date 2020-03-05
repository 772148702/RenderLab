#pragma once

#include <Engine/Light.h>

namespace CppUtil {
	namespace Engine {
		// �ֲ�����ϵ�У����� -y
		class DirectionalLight : public Light {
		public:
			DirectionalLight(const Ubpa::rgbf &color = Ubpa::rgbf(1.f), float intensity = 1.0f)
				: color(color), intensity(intensity) { }

		public:
			static const Basic::Ptr<DirectionalLight> New(const Ubpa::rgbf &color = Ubpa::rgbf(1.f), float intensity = 1.0f) {
				return Basic::New<DirectionalLight>(color, intensity);
			}

		protected:
			virtual ~DirectionalLight() = default;

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
			virtual float PDF(const Ubpa::pointf3 & p, const Ubpa::normalf & wi) const override { return 0.f; }

			virtual bool IsDelta() const override { return true; }

		public:
			Ubpa::rgbf color;
			float intensity;
		};
	}
}
