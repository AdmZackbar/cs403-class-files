#include <stdio.h>
#include <ctype.h>  // For isspace
#include "scanner.h"

static void skipWhitespace(FILE *fp);

char readChar(FILE *fp)
{
    skipWhitespace(fp);

    return fgetc(fp);
}

void pushbackChar(FILE *fp, int ch)
{
    ungetc(ch, fp);
}

static void skipWhitespace(FILE *fp)
{
    int ch;

    //Read characters until non-whitespace is found
    while((ch = fgetc(fp)) != EOF && isspace(ch))
        continue;

    if(ch != EOF)
        ungetc(ch,fp);
}
