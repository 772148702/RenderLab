#ifndef _CPPUTIL_BASIC_MATH_NORMAL_H_
#define _CPPUTIL_BASIC_MATH_NORMAL_H_

#include <CppUtil/Basic/Vector3.h>
#include <CppUtil/Basic/Point2.h>
#include <CppUtil/Basic/Mat3x3.h>
#include <CppUtil/Basic/Math.h>

namespace CppUtil {
	namespace Basic {
		template <typename T>
		class Normal : public Vector<3, T> {
		public:
			using Vector<3, T>::Vector;

		public:
			const Point<2, T> ToTexcoord() const {
				const float phi = std::atan2(-x, -z) + Math::PI;
				const float theta = acos(y);

				const T u = static_cast<T>(phi / (2.f * Math::PI));
				const T v = static_cast<T>(theta / Math::PI);
				return Point<2, T>(u, v);
			}

			const Normal ToTangent() const {
				const float phi = std::atan2(-x, -z) + Math::PI;

				return Normal(cos(phi), 0, -sin(phi));
			}

			const Mat3x3<T> GenCoordSpace() const {
				auto z = Norm();
				auto h = z;
				if (fabs(h.x) <= fabs(h.y) && fabs(h.x) <= fabs(h.z))
					h.x = 1.0;
				else if (fabs(h.y) <= fabs(h.x) && fabs(h.y) <= fabs(h.z))
					h.y = 1.0;
				else
					h.z = 1.0;

				auto y = h.Cross(z).Norm();
				auto x = z.Cross(y).Norm();

				return Mat3x3<T>(x, y, z);
			}

		public:
			// I ����
			// I �� N ��Ӧ���ǵ�λ����
			// eta �� ����ȳ���
			// �������ȫ���䣬�򷵻� Normalf(static_cast<T>(0))
			static const Normal<T> Refract(const Normal<T> & I, const Normal<T> & N, T eta) {
				const auto dotValue = N.Dot(I);
				const auto k = static_cast<T>(1) - eta * eta * (static_cast<T>(1) - dotValue * dotValue);
				if (k < static_cast<T>(0))
					return Normalf(static_cast<T>(0));
				else
					return eta * I - (eta * dotValue + sqrt(k)) * N;
			}

			// I ���ڣ������ǵ�λ����
			// N Ӧ���ǵ�λ����
			// ����ֵ�� length ͬ�� I
			static const Normal<T> Reflect(const Normal<T> & I, const Normal<T> & N) {
				return I - N * N.Dot(I) * static_cast<T>(2);
			}
		};
	}
	template<typename T>
	using Normal = Basic::Normal<T>;

	using Normalf = Normal<float>;
}

#endif // !_CPPUTIL_BASIC_MATH_NORMAL_H_
