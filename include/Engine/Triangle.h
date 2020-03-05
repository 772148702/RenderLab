#pragma once

#include <Engine/Shape.h>

namespace CppUtil {
	namespace Engine {
		class TriMesh;

		// ��Ϊ Triangle ������ Mesh������ Mesh �ֿ���ȡ�� Triangle
		// ���Բ��� Triangle ���� Primitive��ֻ�� Mesh ���� Primitive
		class Triangle final : public Shape {
			friend class TriMesh;
		public:
			Triangle(unsigned idx0, unsigned idx1, unsigned idx2)
				: mesh(Basic::Ptr<TriMesh>(nullptr)), idx{ idx0, idx1, idx2 } { }

		public:
			static const Basic::Ptr<Triangle> New(unsigned idx0, unsigned idx1, unsigned idx2) {
				return Basic::New<Triangle>(idx0, idx1, idx2);
			}

		protected:
			virtual ~Triangle() = default;

		public:
			const Basic::Ptr<TriMesh> GetMesh() const { return mesh.lock(); }
			virtual const Ubpa::bboxf3 GetBBox() const override;
			virtual const Basic::Ptr<Primitive> GetPrimitive() override {
				return Basic::CastTo<Primitive>(mesh.lock());
			}

		public:
			float GetArea() const;

		public:
			unsigned idx[3]; // index into the mesh attribute arrays

		private:
			Basic::WPtr<TriMesh> mesh;
		};
	}
}
