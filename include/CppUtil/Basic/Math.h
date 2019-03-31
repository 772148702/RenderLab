#ifndef _BASIC_MATH_MATH_H_
#define _BASIC_MATH_MATH_H_

#include <glm/glm.hpp>
#include <vector>
#include <sstream>
#include <string>

namespace CppUtil {
	namespace Basic {
		namespace Math {
			constexpr float EPSILON = 1e-6f;
			constexpr float PI = 3.14159265358979323f;
			constexpr float INV_PI = 0.318309886183790672f;

			template<typename T, typename U>
			inline T Lerp(T v0, T v1, U t) {
				return (static_cast<U>(1) - t)*v0 + t * v1;
			}

			inline float Radians(const float degree) {
				return (PI / 180.f) * degree;
			}
			inline float Degrees(const float radians) {
				return (180.f / PI) * radians;
			}

			glm::vec2 RandInCircle();

			glm::vec3 RandInSphere();

			// [-0x7FFFFFFFF, 0x7FFFFFFF]
			int Rand_I();

			// [0, 0xFFFFFFFF]
			unsigned int Rand_UI();

			// [0.0f, 1.0f]
			float Rand_F();
			// [0.0f, 1.0f]
			float Rand_F_exclude1();

			// [0.0, 1.0]
			double Rand_D();

			void RandSetSeedByCurTime();

			// dor( (0.299, 0.587, 0.114), color )
			float Gray(const glm::vec3 & color);

			// ����
			// rationNiNt ������֮�� == Ni / Nt
			//     Ni Ϊ ����������ڽ��ʵ�������
			//     Nt Ϊ ����������ڽ��ʵ�������
			bool Refract(const glm::vec3 & viewDir, const glm::vec3 & normal, float ratioNiNt, glm::vec3 & refractDir);

			// ������
			// viewDir Ϊ ���߷���(����㵽�ӵ�ķ���)
			// halfway Ϊ ���߷�������߷����ƽ��
			// rationNtNi ������֮�� == Nt / Ni
			//     Ni Ϊ ����������ڽ��ʵ�������
			//     Nt Ϊ ����������ڽ��ʵ�������
			float FresnelSchlick(const glm::vec3 & viewDir, const glm::vec3 & halfway, float ratioNtNi);

			template <typename T>
			T Mean(const std::vector<T> & data);

			template<typename T>
			T Variance(const std::vector<T> & data);

			// �������Ԫ�ص�λ��, ��λ�õĸ���ƽ��
			template<typename T>
			void Permute(std::vector<T> data);

			// ����ֵ: (alpah, beta, gamma, t)
			// e + t * d == alpha * a + beta * b + gamma * c
			// alpha + beta + gammma == 1
			// ���ƽ��, ���� (0, 0, 0, 0)
			glm::vec4 Intersect_RayTri(const glm::vec3 & e, const glm::vec3 & d, const glm::vec3 & a, const glm::vec3 & b, const glm::vec3 & c);

			template<typename T>
			T min(const std::vector<T> & val);

			template<typename T>
			T max(const std::vector<T> & val);

			bool IsBase2(int n);

			// return object to world 3x3 matrix
			// n �ǵ�λ����
			glm::mat3 GenCoordSpace(const glm::vec3 & n);

			template<typename Type>
			Type CastTo(const std::string & str) {
				std::istringstream iss(str);
				Type num;
				iss >> num;
				return num;
			}

#include <CppUtil/Basic/Math.inl>
		}
	}
}

#endif // !_BASIC_MATH_MATH_H_
