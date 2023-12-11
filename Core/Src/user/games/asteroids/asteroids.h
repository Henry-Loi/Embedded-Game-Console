#ifndef ASTEROIDS_H
#define ASTEROIDS_H

#include "vector.h"
#define VERTS 10

typedef enum { SMALL, MEDIUM, LARGE } Sizes_t;

typedef struct {
	int alive;
	Sizes_t size;
	float hit_radius;
	float rotation;
	Vec2 location;
	Vec2 velocity;
	Vec2 obj_vert[VERTS];
	Vec2 world_vert[VERTS];
} Asteroid_t;

void init_asteroids(Asteroid_t asteroids[], int size);
void update_asteroids(Asteroid_t asteroids[], Asteroid_t last_asteroids[], int size);
void draw_asteroids(Asteroid_t asteroids[], Asteroid_t last_asteroids[], int size);
int shrink_asteroid(Asteroid_t* a, int size);
void spawn_asteroids(Asteroid_t a[], int length, int size, Vec2 v);
void bounds_asteroids(Asteroid_t asteroids[], int size);
int collision_asteroids(Asteroid_t asteroids[], int size, Vec2* v, float radius);

#endif