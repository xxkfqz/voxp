/*
 * Just do what the fuck you want
 *
 * </> with <3 by xxkfqz <xxkfqz@gmail.com> 2d19
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <dlfcn.h>
#include <signal.h>
#include <getopt.h>

#include "errexit.h"
#include "sunapi.h"
#include "sunvox.h"

#define arrSize(A) (sizeof(A) / sizeof(A[0]))
///////////////////////////////////////////////////////////
typedef struct
{
	int32_t volume;
	int32_t inputFilesNumber;
	int32_t frequency;
	char **inputFiles;
	char *libPath;
	char *exportFilename;

	bool hiresSound:1;
	bool libDebug:1;
	bool monoMode:1;
	bool random:1;
	// 0 - don't repeat
	// 1 - repeat track
	// 2 - repeat list
	uint8_t repeatMode:2;
} commandLineOptions;
///////////////////////////////////////////////////////////
void signalHandler(int32_t param);
void parseArguments(int argc, char **argv, commandLineOptions *ops);
void usage(void);
///////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	if(argc < 2)
		usage();

	// Because someone can use htop
	signal(SIGTERM, signalHandler);
	signal(SIGINT, signalHandler);

	srand((unsigned)time(NULL));

	// Setting up
	commandLineOptions *optionsList = (commandLineOptions*)malloc(sizeof(commandLineOptions));
	// Default settings
	*optionsList = (commandLineOptions)
	{
		.hiresSound = false,
		.libDebug = false,
		.repeatMode = 0,
		.random = false,
		.volume = 255,
		.monoMode = false,
		.frequency = 44100,
		.libPath = NULL,
		.exportFilename = NULL
	};

	parseArguments(argc, argv, optionsList);

	if(optionsList->inputFilesNumber <= 0)
		errexit("No file specified. Try '%s -h'\n", argv[0]);

	bool isExport = optionsList->exportFilename != NULL;
	uint32_t flags = 
		// Export mode
		((isExport ?
			(SV_INIT_FLAG_USER_AUDIO_CALLBACK |
			SV_INIT_FLAG_ONE_THREAD)
			: 0
		)
		// Hires mode
		| (optionsList->hiresSound ?
			SV_INIT_FLAG_AUDIO_FLOAT32 :
			SV_INIT_FLAG_AUDIO_INT16
		)
		// Print debug information?
		| (optionsList->libDebug ?
			0 :
			SV_INIT_FLAG_NO_DEBUG_OUTPUT
	));
	
	sa_initLib(
		optionsList->libPath,
		optionsList->monoMode,
		optionsList->frequency,
		optionsList->hiresSound,
		flags
	);

	float volumePercents = (float)optionsList->volume / 256 * 100;
	int32_t currentTrack = optionsList->random ? rand() % optionsList->inputFilesNumber : 0;
	// Main cycle
	for(
		;
		currentTrack < optionsList->inputFilesNumber;
		currentTrack++
	)
	{
		printf(
			"\n%-15s%d/%d\n%-15s%d Hz, %s, %d-bit\n%-15s%d / %xh / %.1f%%",

			"TRACK",
			currentTrack + 1,
			optionsList->inputFilesNumber,

			"AUDIO",
			optionsList->frequency,
			optionsList->monoMode ? "mono" : "stereo",
			optionsList->hiresSound ? 32 : 16,

			"VOLUME",
			optionsList->volume,
			optionsList->volume,
			volumePercents
		);
		sa_openTrack(
			optionsList->inputFiles[currentTrack],
			optionsList->volume,
			optionsList->repeatMode == 1 ? false : true
		);
		sa_printTrackInfo(0);

		if(isExport)
			sa_exportTrack(0, optionsList->exportFilename);
		else
			sa_playTrack(0);

		// Queue
		if(optionsList->random)
		{
			int32_t lastTrack = currentTrack;
			do
				currentTrack = rand() % optionsList->inputFilesNumber;
			while(lastTrack == currentTrack);
		}
		else if(
			currentTrack >= optionsList->inputFilesNumber - 1 &&
			optionsList->repeatMode == 2
		)
			currentTrack = -1;
	}

	// One can reach this only if repeatMode == 0
	sa_deinitLib();
	return 0;
}

void signalHandler(int32_t param)
{
	sa_deinitLib();
	errexit("\n", param);
}

void parseArguments(int argc, char **argv, commandLineOptions *ops)
{
	const char *optString = "hv:qsrRmf:l:e:";
	const struct option getoptOpsList[] =
	{
		{"help", no_argument, NULL, 'h'},
		{"volume", required_argument, NULL, 'v'},
		{"high-quality", no_argument, NULL, 'q'},
		{"random", no_argument, NULL, 'R'},
		{"repeat-all", no_argument, NULL, 'r'},
		{"repeat-track", no_argument, NULL, 's'},
		{"mono", required_argument, NULL, 'm'},
		{"frequency", required_argument, NULL, 'f'},
		{"lib", required_argument, NULL, 'l'},
		{"export", required_argument, NULL, 'e'},
		{"debug", no_argument, NULL, 0},
		{NULL, 0, NULL, 0}
	};

	char optResult = 0;
	int32_t longIndex;

	while((optResult = getopt_long(argc, argv, optString, getoptOpsList, &longIndex)) != -1)
	{
		switch(optResult)
		{
			case 'v':
				ops->volume = (int32_t)atoi(optarg);
				break;
			case 'q':
				ops->hiresSound = true;
				break;
			case 's':
				ops->repeatMode = 1;
				break;
			case 'r':
				if(ops->repeatMode == 1)
					break;
				ops->repeatMode = 2;
				break;
			case 'R':
				ops->random = true;
				break;
			case 'm':
				ops->monoMode = true;
				break;
			case 'f': {
				int32_t freq = atoi(optarg);
				if(
					freq != 44100 &&
					freq != 48000 &&
					freq != 96000 &&
					freq != 192000
				)
					errexit(
						"Unsupported frequency: %d\n",
						freq
					);
				ops->frequency = freq;
				break;
			}
			case 'l':
				ops->libPath = optarg;
				break;
			case 'e':
				ops->exportFilename = optarg;
				break;
			case 0:
				if(strcmp("debug", getoptOpsList[longIndex].name) == 0)
					ops->libDebug = true;
				break;
			case 'h':
			case '?':
				usage();
				exit(0);
				break;
		}
	}
	ops->inputFiles = argv + optind;
	ops->inputFilesNumber = argc - optind;
}

void usage(void)
{
	const char *line1[] = {
		" - player for SunVox projects with command-line interface",
		"Author: xxkfqz <xxkfqz@gmail.com> 2019",
		""
	};
	const char *line2[] = {
		" [[-hqsrRm] [-v VOLUME] [-f FREQUENCY] [-e EXPORT_FILE_NAME]",
		"     [-l PATH_TO_LIB] [--debug]] *.SUNVOX [*.SUNVOX] ...\'",
		"",
		"Options:",
		"  -h, --help",
		"      see this text and exit",
		"  -v <volume>, --volume <volume>",
		"      playback volume",
		"      (e.g. 256 -> 100%, 128 -> 50%, 512 -> 200%)",
		"  -e <name>, --export <name>",
		"      export track to WAV file",
		"  -q, --high-quality",
		"      hi-res float 32-bit sound instead default integer 16-bit",
		"  -s, --repeat-one",
		"      repeat one track",
		"  -r, --repeat-list",
		"      repeat tracklist (doesn't works with '-s')",
		"  -R, --random",
		"      play tracks randomly (includes '-r')",
		"  -m, --mono",
		"      play tracks with single channel",
		"  -f <frequency>, --frequency <frequency>",
		"      output sample rate in Hz",
		"      Supported rates: 44100, 48000, 96000, 192000",
		"      Default: 44100. High value (e.g. 192000) requires fast CPU",
		"  -l <path_to_lib>, --lib <path_to_lib>",
		"      path to sunvox library (e.g. 'sunvox.so' or 'sunvox_lofi.so')",
		"  --debug",
		"      show engine debug information",
		"",
		"Powered by:",
		"  * SunVox modular synthesizer",
		"    Copyright (c) 2008 - 2018",
		"    Alexander Zolotov <nightradio@gmail.com>, WarmPlace.ru",
		"",
		"  * Ogg Vorbis 'Tremor' integer playback codec",
		"    Copyright (c) 2002",
		"    Xiph.org Foundation"
	};

	uint8_t s = 0;
	printf(PACKAGE_NAME);
	for(; s < arrSize(line1); s++)
		printf("%s\n", line1[s]);
	printf(PACKAGE_NAME);
	for(s = 0; s < arrSize(line2); s++)
		printf("%s\n", line2[s]);

	exit(0);
}
