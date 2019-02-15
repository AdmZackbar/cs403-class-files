#ifndef __SCANNER_INCLUDED__
#define __SCANNER_INCLUDED__

#include <stdio.h>

extern char readChar(FILE *fp, int *lineNum);
extern void skipWhitespace(FILE *fp, int *lineNum);
extern void pushbackChar(FILE *fp, int ch, int *lineNum);

#endif
