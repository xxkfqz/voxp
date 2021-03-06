#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <stdint.h>
#include <stdbool.h>

#include "sunapi.h"
#include "errexit.h"
#define SUNVOX_MAIN
#include "sunvox.h"

#include "export.h"

int32_t sunvoxFrequency = 0;
uint8_t sunvoxChannels = 0;
bool sunvoxHires = false;

void sa_initLib(const char *path, bool monoMode, int32_t frequency, bool isHires, uint32_t initFlags)
{
	if(path == NULL)
	{
		if(sv_load_dll())
			errexit("Cannot load sunvox library\n");
	}
	else
	{
		if(sv_load_dll2(path))
			errexit("\n");
	}

	int32_t ver = sv_init(0, frequency, monoMode, initFlags);
	sunvoxFrequency = frequency;
	sunvoxChannels = monoMode ? 1 : 2;
	sunvoxHires = isHires;
	if(ver < 0)
		errexit("sv_init error: %d\n", ver);

	printf(
		"SunVox library version: %d.%d.%d\n",
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

void sa_openTrack(const char *trackname, int32_t volume, bool repeatMode)
{
	if(sv_load(0, trackname))
		errexit("Cannot load file: %s\n", trackname);

	sv_set_autostop(0, repeatMode);
	sv_volume(0, volume);
}

void sa_printTrackInfo(int32_t slot)
{
	uint32_t minutes, seconds;
	seconds = sv_get_song_length_frames(slot) / sunvoxFrequency;
	minutes = seconds / 60;
	seconds %= 60;


	printf(
		"\n%-15s\"%s\"\n%-15s%d\n%-15s%d:%02d\n%-15s%d\n",

		"TITLE",	
		sv_get_song_name(slot),

		"MODULES",
		sv_get_number_of_modules(slot),

		"LENGHT",
		minutes, seconds,

		"LINES",
		sv_get_song_length_lines(slot)
	);
}

void sa_playTrack(int32_t slot)
{
	sv_play_from_beginning(slot);
	while(!sv_end_of_song(slot))
		sleep(1);
}

void sa_exportTrack(int32_t slot, const char *filename)
{
	exportToWav(slot, filename, sunvoxChannels, sunvoxFrequency, sunvoxHires);
}
