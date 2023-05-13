
CC = wcc386
LINK = wlink

CFLAGS = -omaxnet -zp4 -5r -fp5

MINIMUM_SDL = minimal_sdl.c
MINIMUM_OFFSCREEN = minimal_offscreen.c

all: minimal_sdl minimal_offscreen

minimal_offscreen: .SYMBOLIC
	$(CC) $(CFLAGS) $(MINIMUM_OFFSCREEN)
	$(LINK) &
		FIL minimal_offscreen.o &
		LIBR thirdparty/brender/lib/brstwxrr.lib &
		LIBR thirdparty/brender/lib/brfwwxrr.lib &
		LIBR thirdparty/brender/lib/brfmwxrr.lib &
		LIBR thirdparty/brender/lib/brzbwxrr.lib &

minimal_sdl: .SYMBOLIC
	$(CC) -ei -bt=nt $(CFLAGS) $(MINIMUM_SDL)
	$(LINK) &
	SYS nt_win &
		NAM minimal_sdl.exe &
		FIL minimal_sdl.o &
		LIBF thirdparty/sdl/lib/SDLmain.lib &
		LIBR thirdparty/sdl/lib/SDL.lib &
		LIBR thirdparty/brender/lib/brstwxrr.lib &
		LIBR thirdparty/brender/lib/brfwwxrr.lib &
		LIBR thirdparty/brender/lib/brfmwxrr.lib &
		LIBR thirdparty/brender/lib/brzbwxrr.lib &
