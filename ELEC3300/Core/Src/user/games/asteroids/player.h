#ifndef PLAYER_H
#define PLAYER_H

#include "vector.h"

#include <stdbool.h>

#define P_VERTS 3
#define BULLETS 3

typedef struct {
	Vec2 location;
	Vec2 velocity;
	bool alive;
} Bullet_t;

typedef struct {
	float hit_radius;
	int lives;
	Vec2 location;
	Vec2 velocity;
	Vec2 obj_vert[P_VERTS];
	Vec2 world_vert[P_VERTS];
	Vec2 old_world_vert[P_VERTS];
	Bullet_t bullets[BULLETS];
} Player_t;

void init_player(Player_t* p);
void draw_player(Player_t* p);
void shoot_bullet(Player_t* p);
void update_player(Player_t* p);
void bounds_player(Player_t* p);
void apply_force(Vec2* velocity, Vec2 v);
void rotate_player(Player_t* p, float degrees);
Vec2 get_direction(Player_t* p);

#endif