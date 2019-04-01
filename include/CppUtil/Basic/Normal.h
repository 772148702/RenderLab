#ifndef _CPPUTIL_BASIC_MATH_NORMAL_H_
#define _CPPUTIL_BASIC_MATH_NORMAL_H_

#include <CppUtil/Basic/Vector.h>
#include <CppUtil/Basic/Point2.h>

#include <CppUtil/Basic/Math.h>

namespace CppUtil {
	namespace Basic {
		template <typename T>
		class Normal : public Vector<T> {
		public:
			using Vector::Vector;

		public:
			const Point2<T> ToTexcoord() const {
				const float phi = std::atan2(-x, -z) + Math::PI;
				const float theta = acos(y);

				const T u = static_cast<T>(phi / (2.f * Math::PI));
				const T v = static_cast<T>(theta / Math::PI);
				return Point2<T>(u, v);
			}

			const Normal ToTangent() const {
				const float phi = std::atan2(-x, -z) + Math::PI;

				return Normal(cos(phi), 0, -sin(phi));
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
	using Normalf = Basic::Normal<float>;
}

#endif // !_CPPUTIL_BASIC_MATH_NORMAL_H_
