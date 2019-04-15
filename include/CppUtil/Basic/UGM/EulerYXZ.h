#ifndef _CPPUTIL_BASIC_MATH_UGM_EULER_YXZ_H_
#define _CPPUTIL_BASIC_MATH_UGM_EULER_YXZ_H_

#include <CppUtil/Basic/UGM/Quat.h>

#include <CppUtil/Basic/UGM/ext/Basic_Val3.h>

namespace CppUtil {
	namespace Basic {
		template <typename T>
		class EulerYXZ : public EXT::Basic_Val<3,T, EulerYXZ<T>> {
		public:
			using EXT::Basic_Val<3, T, EulerYXZ<T>>::Basic_Val;

		public:
			const Quat<T> ToQuat() const {
				return Quat<T>::Rotate(*this);
			}

#ifndef NDEBUG
			// ������ Debug ʱ������ IDE ����ʾ���
		public:
			EulerYXZ & operator =(const EulerYXZ & v) {
				*static_cast<EXT::Basic_Val<3, T, EulerYXZ<T>>*>(this) = v;
				return *this;
			}
		private:
			const T & _x{ *(((T*)this) + 0) };
			const T & _y{ *(((T*)this) + 1) };
			const T & _z{ *(((T*)this) + 2) };
#endif // !NDEBUG
		};
	}

	template<typename T>
	using EulerYXZ = Basic::EulerYXZ<T>;

	using EulerYXZf = EulerYXZ<float>;
}

#endif // !_CPPUTIL_BASIC_MATH_UGM_EULER_YXZ_H_
