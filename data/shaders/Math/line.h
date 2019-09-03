#ifndef MATH_LINE_H
#define MATH_LINE_H

// ------------------------------ �ӿ� ------------------------------

struct Line {
	vec3 o;
	vec3 d;
};

vec3 Line_At(Line line, float t);

// ------------------------------ ʵ�� ------------------------------

vec3 Line_At(Line line, float t) {
	return line.o + t * line.d;
}

#endif // !MATH_LINE_H
