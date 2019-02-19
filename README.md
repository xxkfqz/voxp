# voxp

Player for SunVox projects with command-line interface

# Dependecies

* [SunVox library for developers](http://warmplace/ru/soft/sunvox)
* *NIX-like OS

# Usage

```
voxp [-hqsrRm] [-v VOLUME] [-f FREQUENCY] [-e EXPORT_FILE_NAME]
     [-l PATH_TO_LIB] [--debug] *.SUNVOX [*.SUNVOX] ...

Options:
  -h, --help
      see this text and exit
  -v <volume>, --volume <volume>
      playback volume (255 -> 100% (default), 385 -> 150%, etc.)
  -e <name>, --export <name>
      export track to WAV file
  -q, --high-quality
      hi-res float 32-bit sound instead default integer 16-bit
  -s, --repeat-one
      repeat one track
  -r, --repeat-list
      repeat tracklist (doesn't works with '-s')
  -R, --random
      play tracks randomly (includes '-r')
  -m, --mono
      play tracks with single channel
  -f <frequency>, --frequency <frequency>
      output sample rate in Hz. Supported rates: 44100, 48000, 96000, 192000
      Default: 44100. High value (e.g. 192000) may occurs errors
  -l <path_to_lib>, --lib <path_to_lib>
      path to sunvox library (e.g. 'sunvox.so' or 'sunvox_lofi.so')
  --debug
      show engine debug information

Powered by:
  * SunVox modular synthesizer
    Copyright (c) 2008 - 2018
    Alexander Zolotov <nightradio@gmail.com>, WarmPlace.ru

  * Ogg Vorbis 'Tremor' integer playback codec
    Copyright (c) 2002
    Xiph.org Foundation
```
