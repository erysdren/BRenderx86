/*
 * Copyright (c) 1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id:  $
 * $Locker:  $
 *
 */

/* std */
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

/* brender */
#include "thirdparty/brender/inc/brender.h"

/* sdl */
#include "thirdparty/sdl/inc/SDL.h"

/*
 * The screen, offscreen buffer, and the depth buffer
 */
br_pixelmap *screen_buffer, *back_buffer, *depth_buffer;

/*
 * The actors in the world
 */
br_actor *observer, *world;

/*
 * sdl
 */

SDL_Surface *sdl_window;
SDL_Surface *sdl_texture;
uint32_t *sdl_pixels;
int sdl_width = 800;
int sdl_height = 600;
int sdl_bpp = 32;
int sdl_running = SDL_TRUE;

/*
 * err
 */

int err(const char *s)
{
	printf("%s\n", s);
	return EXIT_FAILURE;
}

/*
 * main
 */

int main()
{
	br_actor *a;
	br_pixelmap *palette;
	int i;

	/*
	 * init sdl
	 */

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		return err("couldn't init brender");

	sdl_window = SDL_SetVideoMode(sdl_width, sdl_height, sdl_bpp, SDL_SWSURFACE);
	if (sdl_window == NULL) return err("couldn't open window");

	SDL_WM_SetCaption("BRender", NULL);

	sdl_pixels = (uint32_t *)malloc(sdl_width * sdl_height * sizeof(uint32_t));
	if (sdl_pixels == NULL) return err("couldn't allocate pixels");

	/* create screen texture */
	sdl_texture = SDL_CreateRGBSurfaceFrom(sdl_pixels, sdl_width, sdl_height, sdl_bpp, sdl_width * (sdl_bpp / 8), 0, 0, 0, 0);
	if (sdl_texture == NULL) return err("couldn't create RGB surface");

	/*
	 * Setup renderer and screen
	 */
	BrBegin();
	screen_buffer = BrPixelmapAllocate(BR_PMT_INDEX_8, sdl_width, sdl_height, NULL, BR_PMAF_NORMAL);
	BrZbBegin(screen_buffer->type, BR_PMT_DEPTH_16);

	/*
	 * Setup CLUT (ignored in true-colour)
	 */
	palette = BrPixelmapLoad("std.pal");
	if (palette == NULL) return err("couldn't load std.pal");

	/*
	 * Allocate other buffers
	 */
	back_buffer = BrPixelmapMatch(screen_buffer, BR_PMMATCH_OFFSCREEN);
	depth_buffer = BrPixelmapMatch(back_buffer, BR_PMMATCH_DEPTH_16);
	back_buffer->origin_x = back_buffer->width / 2;
	back_buffer->origin_y = back_buffer->height / 2;

	/*
	 * Build the a world
	 */
	world = BrActorAllocate(BR_ACTOR_NONE, NULL);
	observer = BrActorAdd(world,BrActorAllocate(BR_ACTOR_CAMERA, NULL));
	BrLightEnable(BrActorAdd(world,BrActorAllocate(BR_ACTOR_LIGHT, NULL)));

	a = BrActorAdd(world,BrActorAllocate(BR_ACTOR_MODEL, NULL));
	a->t.type = BR_TRANSFORM_EULER;
	a->t.t.euler.e.order = BR_EULER_ZXY_S;
	BrVector3Set(&a->t.t.euler.t, BR_SCALAR(0.0), BR_SCALAR(0.0), BR_SCALAR(-5.0));

	/*
	 * Tumble the actor around
	 */
	while (sdl_running == SDL_TRUE)
	{
		/*
		 * poll SDL events
		 */

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					sdl_running = SDL_FALSE;
					break;
			}
		}

		/*
		 * Clear the buffers
		 */
		BrPixelmapFill(back_buffer, 0);
		BrPixelmapFill(depth_buffer, 0xFFFFFFFF);

		BrZbSceneRender(world, observer, back_buffer, depth_buffer);
		BrPixelmapDoubleBuffer(screen_buffer, back_buffer);

		a->t.t.euler.e.a = BR_ANGLE_DEG(i);
		a->t.t.euler.e.b = BR_ANGLE_DEG(i*2);
		a->t.t.euler.e.c = BR_ANGLE_DEG(i*3);

		/* update pixels */
		for (i = 0; i < sdl_width * sdl_height; i++)
		{
			((uint32_t *)sdl_texture->pixels)[i] = ((uint32_t *)palette->pixels)[((uint8_t *)screen_buffer->pixels)[i]];
		}

		SDL_BlitSurface(sdl_texture, NULL, sdl_window, NULL);
		SDL_Flip(sdl_window);
	}

	/*
	 * Close down
	 */
	BrPixelmapFree(screen_buffer);
	BrPixelmapFree(depth_buffer);
	BrPixelmapFree(back_buffer);

	BrZbEnd();
	BrEnd();

	free(sdl_pixels);
	SDL_FreeSurface(sdl_texture);
	SDL_Quit();

	return 0;
}
