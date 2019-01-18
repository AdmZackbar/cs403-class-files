#include "scanner.h"
#include "lexeme.h"
#include "types.h"

LEXEME *lex(FILE *fp)
{
    int ch;

    ch = readChar(fp);

    if(ch == '{')
        return newLEXEME(OBRACE);
}
