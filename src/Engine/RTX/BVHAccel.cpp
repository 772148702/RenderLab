#include <Engine/BVHAccel.h>

#include "BVHNode.h"

#include <Engine/Sphere.h>
#include <Engine/Plane.h>
#include <Engine/TriMesh.h>
#include <Engine/Disk.h>
#include <Engine/Capsule.h>

#include <Engine/CmptGeometry.h>

#include <Engine/SObj.h>

#include <Basic/Visitor.h>
#include <Basic/Timer.h>

using namespace CppUtil;
using namespace CppUtil::Engine;
using namespace CppUtil::Basic;
using namespace std;

// ------------ BVHInitVisitor ------------

class BVHAccel::BVHInitVisitor : public Visitor {
public:
	BVHInitVisitor(BVHAccel * holder)
		: holder(holder) {
		RegMemberFunc<CmptGeometry>(&BVHAccel::BVHInitVisitor::Visit);
		RegMemberFunc<Sphere>(&BVHAccel::BVHInitVisitor::Visit);
		RegMemberFunc<Plane>(&BVHAccel::BVHInitVisitor::Visit);
		RegMemberFunc<TriMesh>(&BVHAccel::BVHInitVisitor::Visit);
		RegMemberFunc<Disk>(&BVHAccel::BVHInitVisitor::Visit);
		RegMemberFunc<Capsule>(&BVHAccel::BVHInitVisitor::Visit);
	}

public:
	unordered_map<Ptr<Shape>, Ubpa::bboxf3> shape2wbbox;

public:
	static const Ptr<BVHInitVisitor> New(BVHAccel * holder) {
		return Basic::New<BVHInitVisitor>(holder);
	}

protected:
	virtual ~BVHInitVisitor() = default;

public:
	void Visit(Ptr<CmptGeometry> geo) {
		auto primitive = geo->primitive;
		if (!primitive)
			return;

		const auto w2l = geo->GetSObj()->GetLocalToWorldMatrix().inverse();
		holder->worldToLocalMatrixes[primitive] = w2l;

		holder->primitive2sobj[geo->primitive] = geo->GetSObj();
		geo->primitive->Accept(This());
	}

	void Visit(Ptr<Sphere> sphere) {
		const auto l2w = holder->GetShapeW2LMat(sphere).inverse();
		holder->shapes.push_back(sphere);
		shape2wbbox[sphere] = l2w * sphere->GetBBox();
	}

	void Visit(Ptr<Plane> plane) {
		const auto l2w = holder->GetShapeW2LMat(plane).inverse();
		holder->shapes.push_back(plane);
		shape2wbbox[plane] = l2w * plane->GetBBox();
	}

	void Visit(Ptr<TriMesh> mesh) {
		const auto l2w = holder->GetShapeW2LMat(mesh).inverse();
		for (auto triangle : mesh->GetTriangles()) {
			holder->shapes.push_back(triangle);
			shape2wbbox[triangle] = l2w * triangle->GetBBox();
		}
	}

	void Visit(Ptr<Disk> disk) {
		const auto l2w = holder->GetShapeW2LMat(disk).inverse();
		holder->shapes.push_back(disk);
		shape2wbbox[disk] = l2w * disk->GetBBox();
	}

	void Visit(Ptr<Capsule> capsule) {
		const auto l2w = holder->GetShapeW2LMat(capsule).inverse();
		holder->shapes.push_back(capsule);
		shape2wbbox[capsule] = l2w * capsule->GetBBox();
	}

private:
	BVHAccel * holder;
};

const Ubpa::transformf & BVHAccel::GetShapeW2LMat(Ptr<Shape> shape) const {
	const auto target = worldToLocalMatrixes.find(shape->GetPrimitive());
	assert(target != worldToLocalMatrixes.cend());

	return target->second;
}

const Ptr<SObj> BVHAccel::GetSObj(Ptr<Shape> shape) const{
	const auto target = primitive2sobj.find(shape->GetPrimitive());
	assert(target != primitive2sobj.cend());

	return target->second;
}

void BVHAccel::Clear() {
	worldToLocalMatrixes.clear();
	primitive2sobj.clear();
	shapes.clear();
	linearBVHNodes.clear();
}

void BVHAccel::Init(Ptr<SObj> root) {
	Clear();

	auto geos = root->GetComponentsInChildren<CmptGeometry>();
	auto initVisitor = BVHInitVisitor::New(this);
	for (auto geo : geos)
		geo->Accept(initVisitor);

	printf("Building BVH...\n");
	Timer timer;
	timer.Start();
	const auto bvhRoot = BVHNode::New(initVisitor->shape2wbbox, shapes, 0, shapes.size());
	LinearizeBVH(bvhRoot);
	timer.Stop();
	printf("BVH build done, cost %f s\n", timer.GetWholeTime());
}

void BVHAccel::LinearizeBVH(Ptr<BVHNode> bvhNode) {
	linearBVHNodes.push_back(LinearBVHNode());
	const auto curNodeIdx = linearBVHNodes.size() - 1;

	if (!bvhNode->IsLeaf()) {
		LinearizeBVH(bvhNode->GetL());
		linearBVHNodes[curNodeIdx].InitBranch(bvhNode->GetBBox(), static_cast<int>(linearBVHNodes.size()), bvhNode->GetAxis());
		LinearizeBVH(bvhNode->GetR());
	}
	else
		linearBVHNodes[curNodeIdx].InitLeaf(bvhNode->GetBBox(), static_cast<int>(bvhNode->GetShapeOffset()), static_cast<int>(bvhNode->GetShapesNum()));
}
