/*
 * Just do what the fuck you want
 *
 * Copyleft by xxkfqz <xxkfqz@gmail.com> 2d19
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dlfcn.h>
#include <signal.h>

#include "errexit.h"
#include "sunapi.h"
///////////////////////////////////////////////////////////
void signalHandler(int32_t param);
///////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	if(argc < 2)
		errexit("Use '%s <track_name>'\n", argv[0]);
	signal(SIGINT, signalHandler);
	// Because someone can use htop
	signal(SIGTERM, signalHandler);

	sa_initLib();

	// Do some shit
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
