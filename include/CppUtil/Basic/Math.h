#ifndef _BASIC_MATH_MATH_H_
#define _BASIC_MATH_MATH_H_

#include <glm/glm.hpp>
#include <vector>

namespace CppUtil {
	namespace Basic {
		namespace Math {
			const float EPSILON = 1e-6f;
			const float PI = 3.14159265358979323f;
			const float INV_PI = 0.318309886183790672f;

			glm::vec2 RandInCircle();

			glm::vec3 RandInSphere();

			// [-0x7FFFFFFFF, 0x7FFFFFFF]
			int Rand_I();

			// [0, 0xFFFFFFFF]
			unsigned int Rand_UI();

			// [0.0f, 1.0f]
			float Rand_F();

			// [0.0, 1.0]
			double Rand_D();

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

			// ����ά����ת��Ϊ��������
			glm::vec2 SphereNormal2Texcoord(const glm::vec3 & normal);
			// ����ķ������������
			glm::vec3 SphereNormal2Tangent(const glm::vec3 & normal);

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

			float Illum(const glm::vec3 & color);

#include <CppUtil/Basic/Math.inl>
		}
	}
}

#endif // !_BASIC_MATH_MATH_H_
