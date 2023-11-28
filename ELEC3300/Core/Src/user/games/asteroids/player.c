#include "player.h"

// #include "renderer.h"
#include "lcd.h"

#include <stdio.h>

void init_player(Player_t* p) {
	int i = 0;
	Vec2 translation = {LCD_WIDTH / 2, LCD_HEIGHT / 2};

	p->hit_radius = 15;
	p->lives = 3;
	p->location.x = 0;
	p->location.y = 0;
	p->velocity.x = 0;
	p->velocity.y = 0;
	p->obj_vert[0].x = 0;
	p->obj_vert[0].y = 1.5;
	p->obj_vert[1].x = -1;
	p->obj_vert[1].y = -1;
	p->obj_vert[2].x = 1;
	p->obj_vert[2].y = -1;

	// convert player verts from object space to world space
	for (i = 0; i < P_VERTS; i++) {
		multiply_vector(&p->obj_vert[i], -1);
		multiply_vector(&p->obj_vert[i], 12);
		add_vector(&p->world_vert[i], &p->obj_vert[i]);
		add_vector(&p->world_vert[i], &translation);
	}

	for (i = 0; i < BULLETS; i++) {
		p->bullets[i].alive = false;
	}
}

void apply_force(Vec2* velocity, Vec2 v) { add_vector(velocity, &v); }

Vec2 get_direction(Player_t* p) {
	Vec2 direction = p->obj_vert[0];
	normalise_vector(&direction);

	return direction;
}

void shoot_bullet(Player_t* p) {
	int i = 0;

	for (i = 0; i < BULLETS; i++) {
		if (!(p->bullets[i].alive)) {
			p->bullets[i].alive = true;
			p->bullets[i].location = p->world_vert[0];
			p->bullets[i].velocity = get_direction(p);
			multiply_vector(&p->bullets[i].velocity, 4.1);
			break;
		}
	}
}

void draw_player(Player_t* p) {
	int i = 0;

	if (p->lives > 0) {
		tft_draw_line(p->world_vert[0].x, p->world_vert[0].y, p->world_vert[1].x, p->world_vert[1].y, WHITE);
		tft_draw_line(p->world_vert[1].x, p->world_vert[1].y, p->world_vert[2].x, p->world_vert[2].y, WHITE);
		tft_draw_line(p->world_vert[2].x, p->world_vert[2].y, p->world_vert[0].x, p->world_vert[0].y, WHITE);
	}

	/*
	//draw vers representing the player
	for (i = 0; i < P_VERTS; i++) {

		draw_pixel(pixel_buffer, p->world_vert[i].x, p->world_vert[i].y, 0xff00ffff);
	}
	*/

	// draw verts representing the bullets
	for (i = 0; i < BULLETS; i++) {
		if (p->bullets[i].alive) {
			tft_draw_point(p->bullets[i].location.x, p->bullets[i].location.y, WHITE);
		}
	}

	/*
	//draw vert representing ships location
	vector2d cpy = {p->location.x, p->location.y};
	vector2d translation = {LCD_WIDTH / 2, LCD_HEIGHT / 2};
	add_vector(&cpy, &translation);

	draw_pixel(pixel_buffer, cpy.x, cpy.y, 0x00ff00ff);
	*/
}

void update_player(Player_t* p) {
	limit_vector(&p->velocity, 2);
	add_vector(&p->location, &p->velocity);

	Vec2 translation = {LCD_WIDTH / 2, LCD_HEIGHT / 2};

	int i = 0;

	for (i = 0; i < P_VERTS; i++) {
		p->world_vert[i] = add_vector_new(&p->obj_vert[i], &p->location);
		add_vector(&p->world_vert[i], &translation);
	}

	for (i = 0; i < BULLETS; i++) {
		add_vector(&p->bullets[i].location, &p->bullets[i].velocity);
	}
}

void rotate_player(Player_t* p, float degrees) {
	int i = 0;

	for (i = 0; i < P_VERTS; i++) {
		rotate_vector(&p->obj_vert[i], degrees);
	}
}

void bounds_player(Player_t* p) {
	int i = 0;

	if (p->location.x < -LCD_WIDTH / 2) {
		p->location.x = LCD_WIDTH / 2;
	}

	if (p->location.x > LCD_WIDTH / 2) {
		p->location.x = -LCD_WIDTH / 2;
	}

	if (p->location.y < -LCD_HEIGHT / 2) {
		p->location.y = LCD_HEIGHT / 2;
	}

	if (p->location.y > LCD_HEIGHT / 2) {
		p->location.y = -LCD_HEIGHT / 2;
	}

	// bullet is out of bounds, reset bullet to be shot again
	// bullets are in world space
	for (i = 0; i < BULLETS; i++) {
		if (p->bullets[i].location.x < 0 || p->bullets[i].location.x >= LCD_WIDTH) {
			p->bullets[i].alive = false;
		}

		if (p->bullets[i].location.y < 0 || p->bullets[i].location.y >= LCD_HEIGHT) {
			p->bullets[i].alive = false;
		}
	}
}
