#include "errexit.h"

void errexit(const char *format, ...)
{
	va_list msg;
	va_start(msg, format);
	vfprintf(stderr, format, msg);
	va_end(msg);

	exit(-1);
}
