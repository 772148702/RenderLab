#ifndef _CPPUTIL_ENGINE_PRIMITIVE_DISK_H_
#define _CPPUTIL_ENGINE_PRIMITIVE_DISK_H_

#include <Engine/Primitive.h>

namespace CppUtil {
	namespace Engine {
		class Disk : public Primitive {
		public:
			Disk() = default;

		public:
			static const Basic::Ptr<Disk> New() {
				return Basic::New<Disk>();
			}

		protected:
			virtual ~Disk() = default;

		public:
			virtual const Ubpa::bboxf3 GetBBox() const override {
				return { {-1,-0.001,-1}, {1,0.001,1} };
			}
		};
	}
}

#endif // !_CPPUTIL_ENGINE_PRIMITIVE_DISK_H_
