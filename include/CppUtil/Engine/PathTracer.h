#ifndef _ENGINE_RTX_PATH_TRACER_H_
#define _ENGINE_RTX_PATH_TRACER_H_

#include <CppUtil/Engine/RayTracer.h>

#include <CppUtil/Basic/UGM/Transform.h>
#include <CppUtil/Basic/UGM/Mat3x3.h>

#include <vector>
#include <map>

namespace CppUtil {
	namespace Engine {
		class Light;
		class BVHAccel;

		class RayIntersector;
		class VisibilityChecker;

		class BSDF;

		// ֻ�����ڵ��߳�
		class PathTracer : public RayTracer {
		public:
			PathTracer();

		public:
			static const Basic::Ptr<PathTracer> New() { return Basic::New<PathTracer>(); }

		protected:
			virtual ~PathTracer() = default;

		public:
			virtual const RGBf Trace(Ray & ray) { return Trace(ray, 0, RGBf(1.f)); }

			virtual void Init(Basic::Ptr<Scene> scene, Basic::Ptr<BVHAccel> bvhAccel) override;

		protected:
			// ray ������������ϵ
			const RGBf Trace(Ray & ray, int depth, RGBf pathThroughput);

		private:
			enum SampleLightMode {
				ALL,
				RandomOne,
			};

			const RGBf SampleLight(
				const Point3 & posInWorldSpace,
				const Mat3f & worldToSurface,
				Basic::Ptr<BSDF> bsdf,
				const Normalf & w_out,
				const Point2 & texcoord,
				SampleLightMode mode
			) const;

			const RGBf SampleLightImpl(
				int lightID,
				const Point3 & posInWorldSpace,
				const Point3 & posInLightSpace,
				const Mat3f & worldToSurface,
				Basic::Ptr<BSDF> bsdf,
				const Normalf & w_out,
				const Point2 & texcoord,
				float factorPD
			) const;

			const RGBf SampleBSDF(
				Basic::Ptr<BSDF> bsdf,
				SampleLightMode mode,
				const Normalf & w_out,
				const Mat3f & surfaceToWorld,
				const Point2 & texcoord,
				const Point3 & hitPos,
				int depth,
				RGBf pathThroughput
			);
		
		public:
			int maxDepth;

		private:
			std::vector<Basic::Ptr<Light>> lights;
			std::map<Basic::Ptr<Light>, int> lightToIdx;
			std::vector<Transform> worldToLightVec;
			std::vector<Transform> lightToWorldVec;

			Basic::Ptr<RayIntersector> rayIntersector;
			Basic::Ptr<VisibilityChecker> visibilityChecker;
		};
	}
}

#endif//!_ENGINE_PATH_TRACER_H_
