#include "asteroids.h"
#include "controller.h"
#include "lcd.h"
#include "player.h"
#include "tft.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define ASTEROIDS 27
#define LIVES	  3

int init(int width, int height);

Asteroid_t asteroids[ASTEROIDS]; // The asteroids
Asteroid_t last_asteroids[ASTEROIDS];
Player_t p;			   // The Player_t
Player_t lives[LIVES]; // Player lives left

void asteroids_thread(void* par) {
	int i = 0;
	int j = 0;
	int offset = 0;
	Vec2 translation = {-LCD_WIDTH / 2, -LCD_HEIGHT / 2};

	// set up icons used to represent Player_t lives
	for (i = 0; i < LIVES; i++) {
		init_player(&lives[i]);
		lives[i].lives = 1;

		// shrink lives
		for (j = 0; j < P_VERTS; j++) {
			divide_vector(&lives[i].obj_vert[j], 2);
		}

		// convert screen space vector into world space
		Vec2 top_left = {20 + offset, 20};
		add_vector(&top_left, &translation);
		lives[i].location = top_left;
		update_player(&lives[i]);
		offset += 20;
	}

	// set up Player_t and asteroids in world space
	init_player(&p);
	init_asteroids(asteroids, ASTEROIDS);
	// lcd_init();

	int sleep = 0;
	int quit = 0;
	// SDL_Event event;
	uint32_t next_game_tick = get_ticks();

	bool ast_do_once = true;
	bool ast_do_once_2 = true;
	uint32_t last_ticks = 0;
	// render loop
	while (quit == 0) {
		osDelay(4);
		// if (state[SDL_SCANCODE_ESCAPE]) {
		// quit = 1;
		// }
		if (curr_page != ASTEROIDS_PAGE) {
			continue;
		}

		if (ast_do_once) {
			tft_clear(BLACK);
			tft_clear_buf();
			tft_update();
			ast_do_once = false;
		}
		p.velocity = (Vec2){.x = ctrller.joystick[L_JOY_X], .y = -ctrller.joystick[L_JOY_Y]};
		rotate_player(&p, ctrller.joystick[R_JOY_X]);
		if (ctrller.button[L_BTN_UP]) {
			p.velocity = (Vec2){.x = 0, .y = -0.7};
		}
		if (ctrller.button[L_BTN_DOWN]) {
			p.velocity = (Vec2){.x = 0, .y = 0.7};
		}
		if (ctrller.button[L_BTN_LEFT]) {
			p.velocity = (Vec2){.x = -0.7, .y = 0};
		}
		if (ctrller.button[L_BTN_RIGHT]) {
			p.velocity = (Vec2){.x = 0.7, .y = 0};
		}

		// if (p.lives > 0) {
		if (ctrller.button[R_BTN_RIGHT]) {
			shoot_bullet(&p);
		}


		draw_player(&p);
		// draw_player(&lives[0]);
		// draw_player(&lives[1]);
		// draw_player(&lives[2]);
		draw_asteroids(asteroids, last_asteroids, ASTEROIDS);
		update_player(&p);
		bounds_player(&p);
		bounds_asteroids(asteroids, ASTEROIDS);

		/* if (ast_do_once_2) {
			// tft_clear(BLACK);
			tft_clear_buf();
			tft_update();
			ast_do_once_2 = false;
		} */
		// int res = collision_asteroids(asteroids, ASTEROIDS, &p.location, p.hit_radius);

		/* 		if (res != -1) {
					p.lives--;
					p.location.x = 0;
					p.location.y = 0;
					p.velocity.x = 0;
					p.velocity.y = 0;

					int i = LIVES - 1;

					for (i = LIVES; i >= 0; i--) {
						if (lives[i].lives > 0) {
							lives[i].lives = 0;
							break;
						}
					}
				}
		 */
		Vec2 translation = {-LCD_WIDTH / 2, -LCD_HEIGHT / 2};

		for (int i = 0; i < BULLETS; i++) {
			// only check for collision for bullets that are shown on screen
			if (p.bullets[i].alive) {
				// convert bullet screen space location to world space to compare
				// with asteroids world space to detect a collision
				Vec2 world = add_vector_new(&p.bullets[i].location, &translation);
				int index = collision_asteroids(asteroids, ASTEROIDS, &world, 1);

				// collision occurred
				if (index != -1) {
					asteroids[index].alive = 0;
					p.bullets[i].alive = false;
					tft_draw_point(p.bullets[i].last_loc.x, p.bullets[i].last_loc.y, BLACK);
					tft_draw_point(p.bullets[i].location.x, p.bullets[i].location.y, BLACK);
					for (int j = 0; j < VERTS; j++) {
						tft_draw_line(last_asteroids[index].world_vert[j].x, last_asteroids[index].world_vert[j].y,
									  last_asteroids[index].world_vert[(j + 1) % VERTS].x,
									  last_asteroids[index].world_vert[(j + 1) % VERTS].y, BLACK);

						tft_draw_line(asteroids[index].world_vert[j].x, asteroids[index].world_vert[j].y,
									  asteroids[index].world_vert[(j + 1) % VERTS].x,
									  asteroids[index].world_vert[(j + 1) % VERTS].y, BLACK);
					}

					if (asteroids[index].size != SMALL) {
						spawn_asteroids(asteroids, ASTEROIDS, asteroids[index].size, asteroids[index].location);
					}
				}
			}
		}

		update_asteroids(asteroids, last_asteroids, ASTEROIDS);

		// draw buffer to the texture representing the screen
		// SDL_UpdateTexture(screen, NULL, pixels, LCD_WIDTH * sizeof(Uint32));

		// draw to the screen
		// SDL_RenderClear(renderer);
		// SDL_RenderCopy(renderer, screen, NULL, NULL);
		// SDL_RenderPresent(renderer);

		// time it takes to render 1 frame in milliseconds
		next_game_tick += 1000 / 60;
		sleep = next_game_tick - get_ticks();

		if (sleep >= 0) {
			// SDL_Delay(sleep);
			// osDelay(sleep);
		}
	}

	// free the screen buffer
	// free(pixels);

	// Destroy window
	// SDL_DestroyWindow(window);

	// Quit SDL subsystems
	// SDL_Quit();
}
/*
int init(int width, int height) {
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());

		return 1;
	}

	// Create window
	SDL_CreateWindowAndRenderer(LCD_WIDTH, LCD_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);

	// set up screen texture
	screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, LCD_WIDTH, LCD_HEIGHT);

	// allocate pixel buffer
	pixels = (Uint32*)malloc((LCD_WIDTH * LCD_HEIGHT) * sizeof(Uint32));


	if (window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());

		return 1;
	}

	if (screen == NULL) {
		printf("Texture could not be created! SDL_Error: %s\n", SDL_GetError());

		return 1;
	}

	if (pixels == NULL) {
		printf("Error allocating pixel buffer");

		return 1;
	}

	return 0;
}
 */