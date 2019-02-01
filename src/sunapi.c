#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <stdint.h>

#include "sunapi.h"
#include "errexit.h"
#define SUNVOX_MAIN
#include "sunvox.h"

void sa_initLib(void)
{
	if(sv_load_dll())
		errexit("Cannot load sunvox library\n");

	// TODO: user defined parameters
	int32_t ver = sv_init(0, 44100, 2,
		SV_INIT_FLAG_NO_DEBUG_OUTPUT
	);
	if(ver < 0)
		errexit("sv_init error: %d\n", ver);

	printf(
		"SunVox library version: %d.%d.%d\n\n",
		(ver >> 16) & 255,
		(ver >> 8) & 255,
		ver & 255
	);

	// NOTE: Only one slot?
	sv_open_slot(0);
}

void sa_deinitLib(void)
{
	sv_deinit();
	sv_unload_dll();
}

void sa_openTrack(const char *trackname)
{
	if(sv_load(0, trackname))
		errexit("Cannot load file: %s\n", trackname);

	// TODO: user defined volume
	sv_volume(0, 64);
}

void sa_printTrackInfo(int32_t slot)
{
	uint32_t minutes, seconds;
	seconds = sv_get_song_length_frames(slot) / 44100;
	minutes = seconds / 60;
	seconds %= 60;


	printf(
		"%s\nModules: %d\nLenght: %d:%d (%d lines)\n",

		sv_get_song_name(slot),

		sv_get_number_of_modules(slot),

		minutes, seconds, sv_get_song_length_lines(slot)
	);
}

void sa_playTrack(int32_t slot)
{
	sv_play_from_beginning(slot);
	while(!sv_end_of_song(slot))
		sleep(1);
}