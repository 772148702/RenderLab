#pragma once

#include <Engine/Intersector.h>
#include <UGM/point.h>
#include <UGM/point.h>
#include <UGM/normal.h>
#include <UGM/bbox.h>

#include <Engine/Ray.h>

namespace Ubpa {
	class Element;

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

			Ptr<SObj> closestSObj;
			normalf n;
			pointf2 texcoord;
			normalf tangent;
			Ptr<Triangle> triangle;
			int idx;
		private:
			friend class RayIntersector;
			bool isIntersect;
		};

	public:
		RayIntersector();

		void Init(Ray* ray);

	protected:
		virtual ~RayIntersector() = default;

	public:
		static const Ptr<RayIntersector> New() { return Ubpa::New<RayIntersector>(); }

	public:
		const Rst& GetRst() { return rst; }

	private:
		// ���� rst������ཻ������޸� ray.tMax
		void Visit(Ptr<BVHAccel> bvhAccel);
		void Visit(Ptr<SObj> sobj);
		void Visit(Ptr<Sphere> sphere);
		void Visit(Ptr<Plane> plane);
		void Visit(Ptr<Triangle> triangle);
		void Visit(Ptr<TriMesh> mesh);
		void Visit(Ptr<Disk> disk);
		void Visit(Ptr<Capsule> capsule);

	private:
		bool Intersect(const bboxf3& bbox, const valf3& invDir) const;

	private:
		Ray* ray;
		Rst rst;
	};
}
