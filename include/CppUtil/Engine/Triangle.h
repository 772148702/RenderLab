#ifndef _ENGINE_PRIMITIVE_TRIANGLE_H_
#define _ENGINE_PRIMITIVE_TRIANGLE_H_

typedef unsigned int uint;

#include <CppUtil/Basic/Element.h>
#include <CppUtil/Basic/BBox.h>

namespace CppUtil {
	namespace Engine {
		class TriMesh;

		// ��Ϊ Triangle ������ Mesh������ Mesh �ֿ���ȡ�� Triangle
		// ���Բ��� Triangle ���� Primitive��ֻ�� Mesh ���� Primitive
		class Triangle : public Basic::Element {
			ELE_SETUP(Triangle)
			friend class TriMesh;
		public:
			Triangle(uint idx0, uint idx1, uint idx2)
				: mesh(Basic::Ptr<TriMesh>(nullptr)), idx{ idx0, idx1, idx2 } { }

			Basic::Ptr<TriMesh> GetMesh() const { return mesh.lock(); }
			const Basic::BBoxf GetBBox() const;
		public:
			uint idx[3]; // index into the mesh attribute arrays

		private:
			Basic::WPtr<TriMesh> mesh;
		};
	}
}

#endif//!_ENGINE_PRIMITIVE_TRIANGLE_H_
