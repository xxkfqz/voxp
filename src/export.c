// Based on Alexander Zolotov's WAV exporting algorithm

#include "export.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "errexit.h"
#include "sunvox.h"

#define BUFFER_SIZE 1024

void exportToWav(int32_t slot, const char *filename, uint8_t channels, int32_t freq, bool isHires)
{
	FILE *output = fopen(filename, "wb");
	if(output == NULL)
		errexit("Cannot open file \"%s\"\n", filename);

	int8_t depth = isHires ? 4 : 2;
	void *buffer = (void*)malloc(BUFFER_SIZE * channels * depth);

	uint32_t songLengthFrames = sv_get_song_length_frames(slot);
	uint32_t songLengthBytes = songLengthFrames * depth * channels;
	uint32_t val;

	// WAVE header metadata
	fwrite((void*)"RIFF", 1, 4, output);
	val = songLengthBytes + 36;
	fwrite( &val, 4, 1, output);
	fwrite((void*)"WAVE", 1, 4, output);

	// WAVE format metadata
	fwrite((void*)"fmt ", 1, 4, output);
	// WAVE_FORMAT_PCM #1
	val = 16;
	fwrite(&val, 4, 1, output);
	// WAVE_FORMAT_PCM #2
	val = isHires ? 3 : 1;
	fwrite(&val, 2, 1, output);
	// Channels
	val = channels;
	fwrite(&val, 2, 1, output);
	// Frames per second (rate)
	val = freq;
	fwrite(&val, 4, 1, output);
	// Bytes per second
	val = freq * channels * depth;
	fwrite(&val, 4, 1, output);
	// Bytes per sample (block align)
	val = channels * depth;
	fwrite(&val, 2, 1, output);
	// Bits per second (WAVE depth)
	val = depth * 8;
	fwrite(&val, 2, 1, output);

	// WAVE data metadata
	fwrite((void*)"data", 1, 4, output);
	fwrite(&songLengthBytes, 4, 1, output);

	// WAVE data
	uint32_t currentFrame = 0;
	uint8_t pos = 0;
	sv_play_from_beginning(slot);
	while(currentFrame < songLengthFrames)
	{
		// Get the next piece of audio
		int32_t framesNumber = BUFFER_SIZE;
		if(currentFrame + framesNumber > songLengthFrames)
			framesNumber = songLengthFrames - currentFrame;
		sv_audio_callback(buffer, framesNumber, 0, sv_get_ticks());
		currentFrame += framesNumber;

		// Save this data to the file
		fwrite(buffer, 1, framesNumber * channels * depth, output);

		// Print progress
		uint8_t newPos = (uint8_t)(((float)currentFrame / (float)songLengthFrames) * 100);
		if(pos != newPos)
		{
			if(pos != 0)
			{
				printf("%2d%%%c", pos, pos % 10 == 0 ? '\n' : ' ');
				fflush(stdout);
			}
			else
				printf("\nExporting as \"%s\":\n", filename);
			pos = newPos;
		}
	}
	puts("done!");
	fclose(output);
	free(buffer);
}
