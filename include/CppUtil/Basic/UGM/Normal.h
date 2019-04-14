#ifndef _CPPUTIL_BASIC_MATH_NORMAL_H_
#define _CPPUTIL_BASIC_MATH_NORMAL_H_

#include <CppUtil/Basic/UGM/ext/Alias.h>
#include <CppUtil/Basic/UGM/ext/Basic_Val3.h>
#include <CppUtil/Basic/UGM/ext/Linearity_3.h>
#include <CppUtil/Basic/UGM/ext/InnerProduct_Euclidean_3.h>
#include <CppUtil/Basic/UGM/ext/Normed_InnerProduct.h>

#include <CppUtil/Basic/UGM/Mat3x3.h>

namespace CppUtil {
	namespace Basic {
		template <typename T>
		class Normal : public EXT::NI_C_IE_L_B_3<T, Normal<T>> {
		public:
			using EXT::NI_C_IE_L_B_3<T, Normal<T>>::NI_C_IE_L_B_3;

		public:
			const Mat3x3<T> GenCoordSpace() const {
				const auto z = Normalize();
				auto h = z;
				if (fabs(h.x) <= fabs(h.y) && fabs(h.x) <= fabs(h.z))
					h.x = 1.0;
				else if (fabs(h.y) <= fabs(h.x) && fabs(h.y) <= fabs(h.z))
					h.y = 1.0;
				else
					h.z = 1.0;

				const auto y = h.Cross(z).Normalize();
				const auto x = z.Cross(y).Normalize();

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
