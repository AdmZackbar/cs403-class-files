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

LEXEME *newLEXEMEerror(char *type, int ch)
{
    LEXEME *l = newLEXEME(type);
    l->sVal = malloc(sizeof(char));
    l->sVal[0] = ch;
    return l;
}

char *getTypeLEXEME(LEXEME *lexeme)
{
    return lexeme->type;
}

void printLEXEME(FILE *fp, LEXEME *lexeme)
{
    fprintf(fp, lexeme->type);
}
