#pragma once

#include <stdint.h>
#include <stdbool.h>

// Every function calls 'errexit' if something goes wrong

// Load library using 'sv_load_dll()' or 'sv_load_dll2(path)' if specified,
// initializate SunVox subsystems, open a slot and prints some information
//
// 'initFlags' defined in 'sunvox.h' as 'SV_INIT_FLAG_AUDIO_*'
void sa_initLib(const char *path, bool monoMode, int32_t frequency, bool isHires, uint32_t initFlags);

// Just safely deinitialize SunVox subsystems
void sa_deinitLib(void);

// Load and set volume
void sa_openTrack(const char *trackname, int32_t volume, bool repeatMode);

// Track name, modules number, length (minutes, seconds, lines)
void sa_printTrackInfo(int32_t slot);

// Run a loop and check reach end of the track one time per second
void sa_playTrack(int32_t slot);

// Export track to WAV (PCM)
void sa_exportTrack(int32_t slot, const char *filename);
