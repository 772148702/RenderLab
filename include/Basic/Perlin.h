#pragma once

#include <UGM/vec.h>

#include <vector>

namespace Ubpa {
	namespace Math {
		class Perlin {
		public:
			static float Noise(const vecf3& p);
			static float Turb(const vecf3& p, size_t depth = 7);
		private:
			static float PerlinInterp(const vecf3 c[2][2][2], float u, float v, float w);
			static std::vector<size_t> GenPermute(size_t n);
			static std::vector<vecf3> GenRandVec(size_t n);

			static std::vector<vecf3> randVec;

			// 0, 1, ... , 255 ���˳��������
			static std::vector<size_t> permuteX;
			static std::vector<size_t> permuteY;
			static std::vector<size_t> permuteZ;
		};
	}
}
