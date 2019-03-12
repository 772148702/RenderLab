#ifndef _ENGINE_RTX_PATH_TRACER_H_
#define _ENGINE_RTX_PATH_TRACER_H_

#include <CppUtil/Engine/RayTracer.h>

#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>

#include <vector>
#include <map>

namespace CppUtil {
	namespace Engine {
		class LightBase;
		class BVHAccel;

		class RayIntersector;
		class VisibilityChecker;

		// ֻ�����ڵ��߳�
		class PathTracer : public RayTracer {
			HEAP_OBJ_SETUP(PathTracer)
		public:
			PathTracer();

			virtual void Init(Basic::Ptr<Scene> scene);

		protected:
			// ray ������������ϵ
			virtual glm::vec3 Trace(Basic::Ptr<Ray> ray, int depth);
		
		public:
			volatile int sampleNumForAreaLight;
			volatile int maxDepth;

		private:
			std::vector<Basic::Ptr<LightBase>> lights;
			std::map<Basic::Ptr<LightBase>, int> lightToIdx;
			std::vector<glm::mat4> worldToLightVec;
			std::vector<glm::mat3> dir_worldToLightVec;// ֻ��Ҫ��ת��������ʹ�� mat3
			std::vector<glm::mat3> dir_lightToWorldVec;// ֻ��Ҫ��ת��������ʹ�� mat3

			Basic::Ptr<BVHAccel> bvhAccel;

			Basic::Ptr<RayIntersector> rayIntersector;
			Basic::Ptr<VisibilityChecker> visibilityChecker;
		};
	}
}

#endif//!_ENGINE_PATH_TRACER_H_
