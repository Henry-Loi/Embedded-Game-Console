#include "vector.h"

#include <math.h>
#include <stdio.h>

void add_vector(Vec2* a, Vec2* b) {
	a->x += b->x;
	a->y += b->y;
}

Vec2 add_vector_new(Vec2* a, Vec2* b) {
	Vec2 new = {a->x + b->x, a->y + b->y};

	return new;
}

void multiply_vector(Vec2* v, float n) {
	v->x *= n;
	v->y *= n;
}

void divide_vector(Vec2* v, float n) {
	v->x /= n;
	v->y /= n;
}

void print_vector(Vec2* a) { printf("x = %f\n y = %f\n", a->x, a->y); }

float magnitude_vector(Vec2* v) {
	float c2 = pow(v->x, 2) + pow(v->y, 2);

	return sqrt(c2);
}

void normalise_vector(Vec2* v) {
	float mag = magnitude_vector(v);

	divide_vector(v, mag);
}

void limit_vector(Vec2* v, float limit) {
	float mag = magnitude_vector(v);

	if (mag > limit) {
		float ratio = limit / mag;
		v->x *= ratio;
		v->y *= ratio;
	}
}

void rotate_vector(Vec2* v, float degrees) {
	// calculate radians
	float angle = degrees * M_PI / 180;
	float sine = sin(angle);
	float cosine = cos(angle);

	// rotation matix
	float matrix[2][2] = {{cosine, -sine}, {sine, cosine}};

	float x = v->x;
	float y = v->y;

	v->x = matrix[0][0] * x + matrix[0][1] * y;
	v->y = matrix[1][0] * x + matrix[1][1] * y;
}
