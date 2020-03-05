#ifndef _CPPUTIL_ENGINE_MATERIAL_MICROFACET_DISTRIBUTION_H_
#define _CPPUTIL_ENGINE_MATERIAL_MICROFACET_DISTRIBUTION_H_

#include <UGM/normal.h>
#include <Engine/SurfCoord.h>

namespace CppUtil {
	namespace Engine {
		class MicrofacetDistribution {
		public:
			// ���߷ֲ�����
			virtual float D(const Ubpa::normalf & wh) const = 0;

			virtual const Ubpa::normalf Sample_wh() const = 0;

			virtual float G1(const Ubpa::normalf & w) const {
				return 1.f / (1.f + Lambda(w));
			}

			// ���κ���
			virtual float G(const Ubpa::normalf & wo, const Ubpa::normalf & wi, const Ubpa::normalf & wh) const {
				if (!SurfCoord::IsVisible(wo, wh) || !SurfCoord::IsVisible(wi, wh))
					return 0.f;

				return 1.f / (1.f + Lambda(wo) + Lambda(wi));
			}

			// ���κ��� Smith
			virtual float G_Smith(const Ubpa::normalf & wo, const Ubpa::normalf & wi, const Ubpa::normalf & wh) const {
				if (!SurfCoord::IsVisible(wo, wh) || !SurfCoord::IsVisible(wi, wh))
					return 0.f;

				return G1(wo) * G1(wi);
			}

			// ����ĸ����ǹ��� wh ��
			// path tracing ����Ҫ�ĸ���Ӧ�ǹ��� wi ��
			float PDF(const Ubpa::normalf & wh) const {
				return D(wh) * abs(wh[2]);
			}

		protected:
			virtual float Lambda(const Ubpa::normalf & w) const = 0;
		};
	}
}

#endif // !_CPPUTIL_ENGINE_MATERIAL_MICROFACET_DISTRIBUTION_H_
