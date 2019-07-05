#ifndef _ENGINE_PRIMITIVE_SPHERE_H_
#define _ENGINE_PRIMITIVE_SPHERE_H_

#include <CppUtil/Engine/Primitive.h>
#include <CppUtil/Basic/UGM/Point2.h>
#include <CppUtil/Basic/UGM/Normal.h>

namespace CppUtil {
	namespace Engine {
		class Sphere final : public Primitive {
		public:
			Sphere() = default;
		public:
			static const Basic::Ptr<Sphere> New() { return Basic::New<Sphere>(); }

		protected:
			virtual ~Sphere() = default;

		public:
			virtual const BBoxf GetBBox() const override {
				return BBoxf(Point3(-1.f), Point3(1.f));
			}

		public:
			struct SphereCoord {
				SphereCoord(float theta, float phi) : theta(theta), phi(phi) {}

				SphereCoord(const Point2 & texcoord)
				 : SphereCoord(texcoord.y * Basic::Math::PI, texcoord.x * 2.f * Basic::Math::PI) { }

				const Normalf ToDir() const;

				float theta;
				float phi;
			};

			// ������תΪ��������
			// theta ��Ӧ v���� 0 �� 1
			// phi ��Ӧ u��z ��Ϊ 0��˳���ַ�������
			static const Point2 TexcoordOf(const Normalf & normal);

			// ������תΪ����
			// u ����
			static const Normalf TangentOf(const Normalf & normal);
		};
	}
}

#endif//!_ENGINE_PRIMITIVE_SPHERE_H_
