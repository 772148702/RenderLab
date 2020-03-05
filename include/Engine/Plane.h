#ifndef _ENGINE_PRIMITIVE_PLANE_H_
#define _ENGINE_PRIMITIVE_PLANE_H_

#include <Engine/Primitive.h>

namespace CppUtil {
	namespace Engine {
		class Plane final : public Primitive {
		public:
			Plane() = default;
		public:
			static const Basic::Ptr<Plane> New() { return Basic::New<Plane>(); }

		protected:
			virtual ~Plane() = default;

		public:
			// primitive �ֲ�����ϵ�ڵ� bbox
			virtual const Ubpa::bboxf3 GetBBox() const override {
				return Ubpa::bboxf3(Ubpa::pointf3(-0.5f, -0.001f, -0.5f), Ubpa::pointf3(0.5f, 0.001f, 0.5f));
			}
		};
	}
}

#endif//!_ENGINE_PRIMITIVE_PLANE_H_
