#ifndef _ENGINE_INTERSECTOR_RAY_INTERSECTOR_H_
#define _ENGINE_INTERSECTOR_RAY_INTERSECTOR_H_

#include <CppUtil/Engine/Intersector.h>
#include <CppUtil/Basic/UGM/Point.h>
#include <CppUtil/Basic/UGM/Point2.h>
#include <CppUtil/Basic/UGM/Normal.h>
#include <CppUtil/Basic/UGM/BBox.h>

#include <CppUtil/Engine/Ray.h>

namespace CppUtil {
	namespace Basic {
		class Element;
	}

	namespace Engine {
		class SObj;

		class Sphere;
		class Plane;
		class Triangle;
		class BVHAccel;
		class BVHNode;

		// Ѱ������Ľ���
		class RayIntersector final : public Intersector {
		public:
			// isIntersect �����ж��� Primitive �Ƿ��ཻ
			// closestSObj ���ڼ�¼�����SObj
			// n ���ڼ�¼������ཻ���ķ���
			struct Rst : public Intersector::Rst {
				friend class RayIntersector;

				Rst(bool isIntersect = false)
					: Intersector::Rst(isIntersect), closestSObj(nullptr), n(0) { }

				Basic::Ptr<SObj> closestSObj;
				Normalf n;
				Point2 texcoord;
				Normalf tangent;
			};

		public:
			RayIntersector();

			void Init(Ray * ray);

		protected:
			virtual ~RayIntersector() = default;
			
		public:
			static const Basic::Ptr<RayIntersector> New() { return Basic::New<RayIntersector>(); }

		public:
			Rst & GetRst() { return rst; }

		private:
			// ���� rst������ཻ������޸� ray.tMax
			void Visit(Basic::Ptr<BVHAccel> bvhAccel);
			void Visit(Basic::Ptr<BVHNode> bvhNode);
			void Visit(Basic::Ptr<SObj> sobj);
			void Visit(Basic::Ptr<Sphere> sphere);
			void Visit(Basic::Ptr<Plane> plane);
			void Visit(Basic::Ptr<Triangle> triangle);

		private:
			bool Intersect(const BBoxf & bbox);
			bool Intersect(const BBoxf & bbox, float & t0, float & t1);

		private:
			Ray * ray;
			Rst rst;
		};
	}
}

#endif//!_ENGINE_INTERSECTOR_RAY_INTERSECTOR_H_