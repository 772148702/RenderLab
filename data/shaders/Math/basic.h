#ifndef MATH_BASIC_H
#define MATH_BASIC_H

const float PI = 3.14159265359;
const float INV_PI = 0.31830988618;

// ------------------------------ �ӿ� ------------------------------

float saturate(float x);

// ------------------------------ ʵ�� ------------------------------

float saturate(float x) {
	return clamp(x, 0, 1);
}

#endif // !MATH_BASIC_H
