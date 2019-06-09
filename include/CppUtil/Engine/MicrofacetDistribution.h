#ifndef _CPPUTIL_ENGINE_MATERIAL_MICROFACET_DISTRIBUTION_H_
#define _CPPUTIL_ENGINE_MATERIAL_MICROFACET_DISTRIBUTION_H_

#include <CppUtil/Basic/UGM/Normal.h>
#include <CppUtil/Engine/SurfCoord.h>

namespace CppUtil {
	namespace Engine {
		class MicrofacetDistribution {
		public:
			// ���߷ֲ�����
			virtual float D(const Normalf & wh) const = 0;

			virtual const Normalf Sample_wh() const = 0;

			virtual float G1(const Normalf & w) const {
				return 1.f / (1.f + Lambda(w));
			}

			// ���κ���
			virtual float G(const Normalf & wo, const Normalf & wi, const Normalf & wh) const {
				if (!SurfCoord::IsVisible(wo, wh) || !SurfCoord::IsVisible(wi, wh))
					return 0.f;

				return 1.f / (1.f + Lambda(wo) + Lambda(wi));
			}

			// ���κ��� Smith
			virtual float G_Smith(const Normalf & wo, const Normalf & wi, const Normalf & wh) const {
				if (!SurfCoord::IsVisible(wo, wh) || !SurfCoord::IsVisible(wi, wh))
					return 0.f;

				return G1(wo) * G1(wi);
			}

			// ����ĸ����ǹ��� wh ��
			// path tracing ����Ҫ�ĸ���Ӧ�ǹ��� wi ��
			float PDF(const Normalf & wh) const {
				return D(wh) * abs(wh.z);
			}

		protected:
			virtual float Lambda(const Normalf & w) const = 0;
		};
	}
}

#endif // !_CPPUTIL_ENGINE_MATERIAL_MICROFACET_DISTRIBUTION_H_
