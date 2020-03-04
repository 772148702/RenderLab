#ifndef _ENGINE_INTERSECTOR_RAY_INTERSECTOR_H_
#define _ENGINE_INTERSECTOR_RAY_INTERSECTOR_H_

#include <Engine/Intersector.h>
#include <Basic/UGM/Point.h>
#include <Basic/UGM/Point2.h>
#include <Basic/UGM/Normal.h>
#include <Basic/UGM/BBox.h>

#include <Engine/Ray.h>

namespace CppUtil {
	namespace Basic {
		class Element;
	}

	namespace Engine {
		// Ѱ������Ľ���
		class RayIntersector final : public Intersector {
		public:
			// isIntersect �����ж��� Primitive �Ƿ��ཻ
			// closestSObj ���ڼ�¼����� SObj
			// n ���ڼ�¼������ཻ���ķ���
			struct Rst {
				bool IsIntersect() const {
					return isIntersect;
				}

				Basic::Ptr<SObj> closestSObj;
				Normalf n;
				Point2 texcoord;
				Normalf tangent;
				Basic::Ptr<Triangle> triangle;
				int idx;
			private:
				friend class RayIntersector;
				bool isIntersect;
			};

		public:
			RayIntersector();

			void Init(Ray * ray);

		protected:
			virtual ~RayIntersector() = default;
			
		public:
			static const Basic::Ptr<RayIntersector> New() { return Basic::New<RayIntersector>(); }

		public:
			const Rst & GetRst() { return rst; }

		private:
			// ���� rst������ཻ������޸� ray.tMax
			void Visit(Basic::Ptr<BVHAccel> bvhAccel);
			void Visit(Basic::Ptr<SObj> sobj);
			void Visit(Basic::Ptr<Sphere> sphere);
			void Visit(Basic::Ptr<Plane> plane);
			void Visit(Basic::Ptr<Triangle> triangle);
			void Visit(Basic::Ptr<TriMesh> mesh);
			void Visit(Basic::Ptr<Disk> disk);
			void Visit(Basic::Ptr<Capsule> capsule);

		private:
			bool Intersect(const BBoxf & bbox, const Val3f & invDir) const;

		private:
			Ray * ray;
			Rst rst;
		};
	}
}

#endif//!_ENGINE_INTERSECTOR_RAY_INTERSECTOR_H_