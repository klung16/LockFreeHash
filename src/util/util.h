#ifndef UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

/*** Functions in simutil.c ***/
/* Print message on stderr */
void outmsg(char *fmt, ...);

/* Generate done message from simulator */
void done();

#define UTIL_H
#endif /* UTIL_H */