/*
 * Just do what the fuck you want
 *
 * </> with <3 by xxkfqz <xxkfqz@gmail.com> 2d19
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <signal.h>
//#include <unistd.h>
#include <getopt.h>

#include "errexit.h"
#include "sunapi.h"
#include "sunvox.h"

#define USAGE_TEXT "\
voxp - player for SunVox projects with command-line interface\n\
\n\
Usage: '%s [-hf] [-v N] <PATH_TO_PROJECT>'\n\
Options:\n\
  -h\n\
      see this text and exit\n\
  -v <volume>\n\
      playback volume (255 -> 100% (default), 511 -> 200%, etc.)\n\
  -f\n\
      hi-res float 32-bit sound instead default integer 16-bit\n\
  -r\n\
      repeat one track\n\
  -R\n\
      repeat tracklist (doesn't works with '-r')\n\
  -D\n\
      show sunvox debug information\n\
\n\
Powered by:\n\
  * SunVox modular synthesizer\n\
    Copyright (c) 2008 - 2018\n\
    Alexander Zolotov <nightradio@gmail.com>, WarmPlace.ru\n\
\n\
  * Ogg Vorbis 'Tremor' integer playback codec\n\
    Copyright (c) 2002\n\
    Xiph.org Foundation\n\
"
///////////////////////////////////////////////////////////
typedef struct
{
	int32_t volume;
	bool hiresSound:1;
	bool libDebug:1;
	// 0 - don't repeat
	// 1 - repeat track
	// 2 - repeat list
	uint8_t repeatMode:2;
	char **inputFiles;
	int32_t inputFilesNumber;
} options;
///////////////////////////////////////////////////////////
void signalHandler(int32_t param);
void parseArguments(int argc, char **argv, options *ops);
///////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	//errexit("%d\n", (int)sizeof(options));
	if(argc < 2)
		errexit("Try '%s -h'\n", argv[0]);

	// Because someone can use htop
	signal(SIGTERM, signalHandler);
	signal(SIGINT, signalHandler);

	options *optionsList = (options*)malloc(sizeof(options));
	// Default settings
	*optionsList = (options)
	{
		.hiresSound = false,
		.libDebug = false,
		.repeatMode = 0,
		.volume = 255
	};

	parseArguments(argc, argv, optionsList);

	sa_initLib(
		(optionsList->hiresSound ?
			SV_INIT_FLAG_AUDIO_FLOAT32 :
			SV_INIT_FLAG_AUDIO_INT16) |
		(optionsList->libDebug ?
			0 :
			SV_INIT_FLAG_NO_DEBUG_OUTPUT)
	);

	for(
		int32_t currentTrack = 0;
		currentTrack < optionsList->inputFilesNumber;
		currentTrack++
	)
	{
		printf(
			"\nTRACK:     %d/%d",
			currentTrack + 1,
			optionsList->inputFilesNumber
		);
		sa_openTrack(
			optionsList->inputFiles[currentTrack],
			optionsList->volume,
			optionsList->repeatMode == 1 ? false : true
		);
		sa_printTrackInfo(0);
		sa_playTrack(0);

		if(
			currentTrack >= optionsList->inputFilesNumber - 1 &&
			optionsList->repeatMode == 2
		)
			currentTrack = -1;
	}

	sa_deinitLib();
	return 0;
}

void signalHandler(int32_t param)
{
	sa_deinitLib();
	errexit("\nProgram has been interrupted with signal %d\n", param);
}

void parseArguments(int argc, char **argv, options *ops)
{
	const char *optString = "hv:frRD";
	char optResult = 0;

	while((optResult = getopt(argc, argv, optString)) != -1)
	{
		switch(optResult)
		{
			case 'v':
				ops->volume = (int32_t)atoi(optarg);
				break;
			case 'f':
				ops->hiresSound = true;
				break;
			case 'R':
				ops->repeatMode = 1;
				break;
			case 'r':
				if(ops->repeatMode == 1)
					break;
				ops->repeatMode = 2;
				break;
			case 'D':
				ops->libDebug = true;
				break;
			case 'h': case '?':
				errexit(USAGE_TEXT, argv[0]);
				break;
		}
	}
	ops->inputFiles = argv + optind;
	ops->inputFilesNumber = argc - optind;
}
