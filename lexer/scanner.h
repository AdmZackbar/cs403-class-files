#ifndef __SCANNER_INCLUDED__
#define __SCANNER_INCLUDED__

#include <stdio.h>

extern char readChar(FILE *fp);

extern void pushbackChar(FILE *fp, int ch);

#endif
