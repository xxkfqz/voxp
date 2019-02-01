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
 ██ ██\n\
 █   █                    voxp\n\
██ █ ██      Player for SunVox projects with\n\
 █   █            command-line interface\n\
 ██ ██\n\
  █ █    </> with <3 by xxkfqz <xxkfqz@gmail.com>\n\
███████                  2019\n\
\n\
Options:\n\
  -h\n\
      see this text and exit\n\
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
void signalHandler(int32_t param);
void parseArguments(int argc, char **argv);
///////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	if(argc < 2)
		errexit("Try '%s -h'\n", argv[0]);

	// Because someone can use htop
	signal(SIGTERM, signalHandler);
	signal(SIGINT, signalHandler);

	parseArguments(argc, argv);

	sa_initLib();

	sa_openTrack(argv[1]);
	sa_printTrackInfo(0);
	sa_playTrack(0);

	sa_deinitLib();
	return 0;
}

void signalHandler(int32_t param)
{
	sa_deinitLib();
	errexit("\nProgram has been interrupted with signal %d\n", param);
}

void parseArguments(int argc, char **argv)
{
	const char *optString = "h";
	char optResult = 0;

	while((optResult = getopt(argc, argv, optString)) != -1)
	{
		switch(optResult)
		{
			case 'h': errexit(USAGE_TEXT); break;
			case '?': break;
		}
	}
}
