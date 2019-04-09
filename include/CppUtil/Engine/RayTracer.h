#ifndef _ENGINE_RTX_RAY_TRACER_H_
#define _ENGINE_RTX_RAY_TRACER_H_

#include <CppUtil/Basic/HeapObj.h>

#include <CppUtil/Engine/Ray.h>

#include <CppUtil/Basic/UGM/RGB.h>

namespace CppUtil {
	namespace Engine {
		class Scene;

		class RayTracer : public Basic::HeapObj {
		protected:
			RayTracer() = default;
			virtual ~RayTracer() = default;

		public:
			// ray ������������ϵ
			virtual const RGBf Trace(Ray & ray) = 0;
			virtual void Init(Basic::Ptr<Scene> scene) {}
		};
	}
}

#endif//!_ENGINE_RTX_RAY_TRACER_H_
