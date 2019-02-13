#ifndef _ENGINE_RTX_PATH_TRACER_H_
#define _ENGINE_RTX_PATH_TRACER_H_

#include <CppUtil/Engine/RayTracer.h>
#include <vector>
#include <glm/mat3x3.hpp>

namespace CppUtil {
	namespace Engine {
		class SObj;
		class LightBase;

		class PathTracer : public RayTracer {
			HEAP_OBJ_SETUP(PathTracer)
		public:
			PathTracer(Basic::Ptr<Scene> scene);

			virtual void Init();

		protected:
			// ray ������������ϵ
			virtual glm::vec3 Trace(Basic::Ptr<Ray> ray, int depth);
		
		public:
			int sampleNumForAreaLight;
			int maxDepth;

		private:
			std::vector<Basic::Ptr<LightBase>> lights;
			std::vector<glm::mat4> worldToLightVec;
			std::vector<glm::mat3> dir_worldToLightVec;// ֻ��Ҫ��ת��������ʹ�� mat3
			std::vector<glm::mat3> dir_lightToWorldVec;// ֻ��Ҫ��ת��������ʹ�� mat3
		};
	}
}

#endif//!_ENGINE_PATH_TRACER_H_
