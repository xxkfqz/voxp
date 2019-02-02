/*
 * Just do what the fuck you want
 *
 * </> with <3 by xxkfqz <xxkfqz@gmail.com> 2d19
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dlfcn.h>
#include <signal.h>
//#include <unistd.h>
#include <getopt.h>

#include "errexit.h"
#include "sunapi.h"

#define USAGE_TEXT "\
voxp - player for SunVox projects with command-line interface\n\
\n\
Usage: '%s [-h] [-v N] <PATH_TO_PROJECT>'\n\
Options:\n\
  -h\n\
      see this text and exit\n\
  -v <volume>\n\
      playback volume (255 -> 100% (default), 511 -> 200%, etc.)\n\
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
} options;
///////////////////////////////////////////////////////////
void signalHandler(int32_t param);
void parseArguments(int argc, char **argv, options *ops);
///////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	if(argc < 2)
		errexit("Try '%s -h'\n", argv[0]);

	// Because someone can use htop
	signal(SIGTERM, signalHandler);
	signal(SIGINT, signalHandler);

	const char *trackname = argv[argc - 1];

	options *optionsList = (options*)malloc(sizeof(options));
	// Default settings
	*optionsList = (options)
	{
		.volume = 255
	};

	parseArguments(argc, argv, optionsList);

	sa_initLib();

	sa_openTrack(trackname, optionsList->volume);
	sa_printTrackInfo(0);
	sa_playTrack(0);

	sa_deinitLib();
	return 0;
}

void signalHandler(int32_t param)
{
	sa_deinitLib();
	//errexit("\nProgram has been interrupted with signal %d\n", param);
	exit(-1);
}

void parseArguments(int argc, char **argv, options *ops)
{
	const char *optString = "hv:";
	char optResult = 0;

	while((optResult = getopt(argc, argv, optString)) != -1)
	{
		switch(optResult)
		{
			case 'h':
				errexit(USAGE_TEXT, argv[0]);
				break;
			case 'v':
				ops->volume = (int32_t)atoi(optarg);
				break;
			case '?':
				errexit("Try '%s -h'\n", argv[0]);
				break;
		}
	}
}
