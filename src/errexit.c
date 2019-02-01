#include "errexit.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void errexit(const char *format, ...)
{
	va_list msg;
	va_start(msg, format);
	vfprintf(stderr, format, msg);
	va_end(msg);

	exit(-1);
}
