#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
typedef struct {
	float x;
	float y;
} Vec2;

void add_vector(Vec2* a, Vec2* b);
Vec2 add_vector_new(Vec2* a, Vec2* b);
void multiply_vector(Vec2* v, float n);
void divide_vector(Vec2* v, float n);
void print_vector(Vec2* a);
void rotate_vector(Vec2* v, float degrees);
float magnitude_vector(Vec2* v);
void normalise_vector(Vec2* v);
void limit_vector(Vec2* v, float limit);
bool equal_vector(Vec2* v1, Vec2* v2);

#endif
