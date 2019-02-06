#!/usr/bin/env python3

import sys, os
from os import system as _

if len(sys.argv) < 2:
    script_name = sys.argv[0]
    print('"{} release" - build\n"{} debug"   - build without optimizations'.format(script_name, script_name))
    sys.exit(-1)

TARGET = sys.argv[1]

if TARGET != 'release' and TARGET != 'debug':
    print('Unknown target "{}"'.format(TARGET))
    sys.exit(-1)

OUTPUTFILE = 'voxp '
COMPILER = 'gcc '
CFLAGS = '-c --std=c11 -I ./include ./src/*.c '
BUILDFLAGS = '-g -O0 -Wall ' if TARGET == 'debug' else '-O3 -march=native '
LINKERFLAGS = '-ldl -lm ./*.o -o {} '.format(OUTPUTFILE)

# Compile
c = COMPILER + CFLAGS + BUILDFLAGS
print(c)
_(c)
# Link
l = COMPILER + LINKERFLAGS
print(l)
_(l)
