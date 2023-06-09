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
#include <stdlib.h>

/* brender */
#include "thirdparty/brender/inc/brender.h"

/*
 * The screen, offscreen buffer, and the depth buffer
 */
br_pixelmap *screen_buffer, *back_buffer, *depth_buffer;

/*
 * The actors in the world
 */
br_actor *observer, *world;


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
	 * Setup renderer and screen
	 */
	BrBegin();
	screen_buffer = BrPixelmapAllocate(BR_PMT_INDEX_8, 320, 200, NULL, BR_PMAF_NORMAL);
	BrZbBegin(screen_buffer->type, BR_PMT_DEPTH_16);

	/*
	 * Setup CLUT (ignored in true-colour)
	 */
	palette = BrPixelmapLoad("std.pal");
	if (palette == NULL) return err("couldn't load std.pal");

	/*
	 * Allocate other buffers
	 */
	back_buffer = BrPixelmapMatch(screen_buffer,BR_PMMATCH_OFFSCREEN);
	depth_buffer = BrPixelmapMatch(back_buffer,BR_PMMATCH_DEPTH_16);
	back_buffer->origin_x = back_buffer->width/2;
	back_buffer->origin_y = back_buffer->height/2;

	/*
	 * Build the a world
	 */
	world = BrActorAllocate(BR_ACTOR_NONE,NULL);
	observer = BrActorAdd(world,BrActorAllocate(BR_ACTOR_CAMERA,NULL));
	BrLightEnable(BrActorAdd(world,BrActorAllocate(BR_ACTOR_LIGHT,NULL)));

	a = BrActorAdd(world,BrActorAllocate(BR_ACTOR_MODEL,NULL));
	a->t.type = BR_TRANSFORM_EULER;
	a->t.t.euler.e.order = BR_EULER_ZXY_S;
	BrVector3Set(&a->t.t.euler.t,BR_SCALAR(0.0),BR_SCALAR(0.0),BR_SCALAR(-5.0));

	/*
	 * Tumble the actor around
	 */
	for (i = 0; i < 360; i++)
	{
		/*
		 * Clear the buffers
		 */
		BrPixelmapFill(back_buffer, 0);
		BrPixelmapFill(depth_buffer, 0xFFFFFFFF);

		BrZbSceneRender(world,observer, back_buffer, depth_buffer);
		BrPixelmapDoubleBuffer(screen_buffer, back_buffer);

		a->t.t.euler.e.a = BR_ANGLE_DEG(i);
		a->t.t.euler.e.b = BR_ANGLE_DEG(i*2);
		a->t.t.euler.e.c = BR_ANGLE_DEG(i*3);
	}

	/*
	 * Close down
	 */
	BrPixelmapSave("minimal_offscreen.pix", screen_buffer);
	BrPixelmapFree(screen_buffer);
	BrPixelmapFree(depth_buffer);
	BrPixelmapFree(back_buffer);

	BrZbEnd();
	BrEnd();

	return 0;
}

