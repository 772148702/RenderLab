#pragma once

#include <Engine/Primitive.h>
#include <UGM/point.h>
#include <UGM/normal.h>

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
			virtual const Ubpa::bboxf3 GetBBox() const override {
				return Ubpa::bboxf3(Ubpa::pointf3(-1.f,-1.f,-1.f), Ubpa::pointf3(1.f,1.f,1.f));
			}

		public:
			struct SphereCoord {
				SphereCoord(float theta, float phi) : theta(theta), phi(phi) {}

				SphereCoord(const Ubpa::pointf2 & texcoord)
				 : SphereCoord(texcoord[1] * Ubpa::PI<float>, texcoord[0] * 2.f * Ubpa::PI<float>) { }

				const Ubpa::normalf ToDir() const;

				float theta;
				float phi;
			};

			// ������תΪ��������
			// theta ��Ӧ v���� 0 �� 1
			// phi ��Ӧ u��z ��Ϊ 0��˳���ַ�������
			static const Ubpa::pointf2 TexcoordOf(const Ubpa::normalf & normal);

			// ������תΪ����
			// u ����
			static const Ubpa::normalf TangentOf(const Ubpa::normalf & normal);
		};
	}
}
