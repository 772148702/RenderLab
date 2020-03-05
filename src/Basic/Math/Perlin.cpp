#include <Basic/Perlin.h>

#include <Basic/Math.h>
#include <UGM/val.h>

using namespace Ubpa;
using namespace Math;
using namespace std;

float Perlin::Noise(const vecf3 & p){
	// pi Ϊ��������
	const auto pi = p.cast_to<vali3>();
	// pf ΪС������
	const auto pf = p - pi.cast_to<vecf3>();
	// 8��λ�õ�����
	// ��λ������8������, ÿ������һ���������
	vecf3 c[2][2][2];
	for (int dx = 0; dx < 2; dx++) {
		for (int dy = 0; dy < 2; dy++) {
			for (int dz = 0; dz < 2; dz++) {
				size_t idx = permuteX[(pi[0] + dx) & 255]
					^ permuteY[(pi[1] + dy) & 255]
					^ permuteZ[(pi[2] + dz) & 255];
				c[dx][dy][dz] = randVec[idx];
			}
		}
	}
	return PerlinInterp(c, pf[0], pf[1], pf[2]);
}

float Perlin::PerlinInterp(const vecf3 c[2][2][2], float u, float v, float w) {
	float uu = u * u*(3 - 2 * u);
	float vv = v * v*(3 - 2 * v);
	float ww = w * w*(3 - 2 * w);
	float sum = 0;
	for (size_t i = 0; i < 2; i++) {
		for (size_t j = 0; j < 2; j++) {
			for (size_t k = 0; k < 2; k++) {
				// ���� �� ��ֵ�� ������
				vecf3 weightVec(u - i, v - j, w - k);
				// Ȩֵ Ϊ ��ֵ�� �� [����Խ�] �� xyz���� �� ����(�Ǹ�) �ĳ˻�
				//vecf3 absWeightVec = abs(vecf3(u-(1-i),v-(1-j),w-(1-k)));
				//float weight = absWeightVec[0] * absWeightVec[1] * absWeightVec[2];
				float weight = (i*uu + (1 - i)*(1 - uu))
					* (j*vv + (1 - j)*(1 - vv))
					* (k*ww + (1 - k)*(1 - ww));
				// ��Ȩ���, ��Ϊ���, �Ӷ��ﵽ����ֵת�Ƶ�����(������������)
				sum += weight * c[i][j][k].dot(weightVec);
			}
		}
	}
	return sum;
}

float Perlin::Turb(const vecf3 & p, size_t depth){
	float sum = 0;
	vecf3 curP = p;
	
	float weight = 1.0;
	for (size_t i = 0; i < depth; i++) {
		sum += weight * Noise(curP);
		// weight = pow(0.5, i);
		weight *= 0.5;
		// curP = p * pow(2, i);
		curP *= 2;
	}
	//float abs --> fabs
	return fabs(sum);
}

vector<size_t> Perlin::GenPermute(size_t n) {
	vector<size_t> rst(n);
	for (size_t i = 0; i < n; i++)
		rst[i] = i;

	Math::Permute(rst);
	return rst;
}

vector<vecf3> Perlin::GenRandVec(size_t n) {
	vector<vecf3> rst(n);
	for (size_t i = 0; i < n; ++i) {
		// sample in SphereMesh
		float Xi1 = Rand_F();
		float Xi2 = Rand_F();
		float Xi3 = Rand_F();

		float cosTheta = 1 - 2.f * Xi1;
		float sinTheta = sqrt(1 - cosTheta * cosTheta);
		float phi = 2 * PI * Xi2;
		float r = pow(Xi3, 1.f / 3.f);

		rst[i][0] = r * sinTheta * cos(phi);
		rst[i][1] = r * sinTheta * sin(phi);
		rst[i][2] = r * cosTheta;
	}

	return rst;
}

vector<vecf3> Perlin::randVec = Perlin::GenRandVec(256);
vector<size_t> Perlin::permuteX = Perlin::GenPermute(256);
vector<size_t> Perlin::permuteY = Perlin::GenPermute(256);
vector<size_t> Perlin::permuteZ = Perlin::GenPermute(256);