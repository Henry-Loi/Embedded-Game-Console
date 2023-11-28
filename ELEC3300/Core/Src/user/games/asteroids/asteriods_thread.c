#include "asteroids.h"
#include "controller.h"
#include "lcd.h"
#include "player.h"

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
	lcd_init();

	int sleep = 0;
	int quit = 0;
	// SDL_Event event;
	uint32_t next_game_tick = get_ticks();

	uint32_t last_ticks = 0;
	// render loop
	tft_clear(BLACK);
	while (quit == 0) {
		osDelay(4);
		// check for new events every frame
		// SDL_PumpEvents();
		// const Uint8* state = SDL_GetKeyboardState(NULL);

		// if (state[SDL_SCANCODE_ESCAPE]) {
		// quit = 1;
		// }

		// if (state[SDL_SCANCODE_UP]) {
		// Vec2 thrust = get_direction(&p);
		// multiply_vector(&thrust, .06);
		// apply_force(&p.velocity, thrust);
		// }
		p.velocity = (Vec2){.x = ctrller.joystick[L_JOY_X], .y = -ctrller.joystick[L_JOY_Y]};

		// if (state[SDL_SCANCODE_LEFT]) {
		// volatile float rotate_angle = atan2(ctrller.joystick[R_JOY_Y], ctrller.joystick[R_JOY_X]);
		rotate_player(&p, ctrller.joystick[R_JOY_X]);
		// }


		// if (state[SDL_SCANCODE_RIGHT]) {
		// rotate_player(&p, 4);
		// }

		// while (SDL_PollEvent(&event)) {
		// switch (event.type) {
		// case SDL_KEYDOWN:

		// switch (event.key.keysym.sym) {
		// case SDLK_SPACE:

		// if (p.lives > 0) {
		// shoot_bullet(&p);
		// }

		// break;
		// }
		// }
		// }
		// draw to the pixel buffer
		draw_player(&p);
		// draw_player(&lives[0]);
		// draw_player(&lives[1]);
		// draw_player(&lives[2]);
		draw_asteroids(asteroids, last_asteroids, ASTEROIDS);
		update_player(&p);
		bounds_player(&p);
		bounds_asteroids(asteroids, ASTEROIDS);

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
		int i = 0;
		Vec2 translation = {-LCD_WIDTH / 2, -LCD_HEIGHT / 2};

		for (i = 0; i < BULLETS; i++) {
			// only check for collision for bullets that are shown on screen
			if (p.bullets[i].alive) {
				// convert bullet screen space location to world space to compare
				// with asteroids world space to detect a collision
				Vec2 world = add_vector_new(&p.bullets[i].location, &translation);
				int index = collision_asteroids(asteroids, ASTEROIDS, &world, 1);

				// collision occured
				if (index != -1) {
					asteroids[index].alive = 0;
					p.bullets[i].alive = false;

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