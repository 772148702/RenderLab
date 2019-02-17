#ifndef _ENGINE_INTERSECTOR_RAY_INTERSECTOR_H_
#define _ENGINE_INTERSECTOR_RAY_INTERSECTOR_H_

#include <CppUtil/Engine/Intersector.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace CppUtil {
	namespace Basic {
		class Element;
	}

	namespace Engine {
		class SObj;
		class Ray;

		class BBox;
		class Sphere;
		class Plane;
		class Triangle;
		class BVHAccel;
		template <typename T, typename HolderT>
		class BVHNode;

		// Ѱ������Ľ���
		class RayIntersector : public Intersector {
			ELEVISITOR_SETUP(RayIntersector)
		public:
			// isIntersect �����ж��� Primitive �Ƿ��ཻ
			// closestSObj ���ڼ�¼�����SObj
			// n ���ڼ�¼������ཻ���ķ���
			struct Rst : public Intersector::Rst {
				friend class RayIntersector;

				Rst(bool isIntersect = false)
					: Intersector::Rst(isIntersect), closestSObj(nullptr), n(0) { }

				Basic::Ptr<SObj> closestSObj;
				glm::vec3 n;
				glm::vec2 texcoord;
			};

		public:
			RayIntersector(Basic::Ptr<Ray> ray);

		public:
			const Rst & GetRst() const { return rst; }

		private:
			// ���� rst������ཻ������޸� ray.tMax
			void Visit(Basic::Ptr<BVHAccel> bvhAccel);
			void Visit(Basic::Ptr<BVHNode<Basic::Element, BVHAccel>> bvhNode);
			void Visit(Basic::Ptr<SObj> sobj);
			void Visit(Basic::Ptr<Sphere> sphere);
			void Visit(Basic::Ptr<Plane> plane);
			void Visit(Basic::Ptr<Triangle> triangle);

		private:
			bool Intersect(const BBox & bbox);
			bool Intersect(const BBox & bbox, float & t0, float & t1);

		private:
			Basic::Ptr<Ray> ray;
			Rst rst;
		};
	}
}

#endif//!_ENGINE_INTERSECTOR_RAY_INTERSECTOR_H_