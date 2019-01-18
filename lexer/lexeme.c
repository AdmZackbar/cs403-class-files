#include <stdlib.h>
#include <assert.h>
#include "lexeme.h"

struct lexeme
{
    char *type;
    int iVal;
    double rVal;
    char *sVal;
};

LEXEME *newLEXEME(char *type)
{
    LEXEME *l = malloc(sizeof(LEXEME));
    assert(l != 0);

    l->type = type;
    return l;
}

char *getTypeLEXEME(LEXEME *lexeme)
{
    return lexeme->type;
}
