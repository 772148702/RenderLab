#ifndef _ENGINE_PRIMITIVE_TRIMESH_H_
#define _ENGINE_PRIMITIVE_TRIMESH_H_

#include <CppUtil/Engine/Primitive.h>
#include <CppUtil/Engine/Triangle.h>
#include <CppUtil/Engine/BVHNode.h>

#include <glm/vec3.hpp>

namespace CppUtil {
	namespace Engine {
		class TriMesh : public Primitive {
			ELE_SETUP(TriMesh)
		public:
			TriMesh(const std::vector<size_t> & indice,
				const std::vector<glm::vec3> & positions,
				const std::vector<glm::vec3> & normals,
				const std::vector<glm::vec3> & texcoords);

			TriMesh(size_t triNum, size_t vertexNum,
				const size_t * indice,
				const float * positions,
				const float * normals,
				const float * texcoords = nullptr);

		public:
			// ���������Ӧ����������
			void InitAfterGenSharePtr();

		public:
			const std::vector<glm::vec3> & GetPositions() const { return positions; }
			const std::vector<glm::vec3> & GetNormals() const { return normals; }
			const std::vector<glm::vec3> & GetTexcoords() const { return texcoords; }
			const std::vector<size_t> & GetIndice() const { return indice; }
			const std::vector<Triangle::Ptr> & GetTriangles() const { return triangles; }

		private:
			std::vector<size_t> indice;
			std::vector<glm::vec3> positions;
			std::vector<glm::vec3> normals;
			std::vector<glm::vec3> texcoords;

			std::vector<Triangle::Ptr> triangles;
		};
	}
}

#endif//!_ENGINE_PRIMITIVE_TRIMESH_H_
