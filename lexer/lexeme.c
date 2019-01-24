#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "lexeme.h"
#include "types.h"

struct lexeme
{
    char *type;
    int lineNum;
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

LEXEME *newLEXEMEstring(char *type, char *value)
{
    LEXEME *l = newLEXEME(type);
    l->sVal = value;
    return l;
}

LEXEME *newLEXEMEdouble(double value)
{
    LEXEME *l = newLEXEME(REAL);
    l->rVal = value;
    return l;
}
LEXEME *newLEXEMEint(int value)
{
    LEXEME *l = newLEXEME(INTEGER);
    l->iVal = value;
    return l;
}

char *getTypeLEXEME(LEXEME *lexeme)
{
    return lexeme->type;
}

int isErrorLEXEME(LEXEME *lexeme)
{
    return lexeme->type == PARSE_ERROR || lexeme->type == BAD_NUM;
}

void printLEXEME(FILE *fp, LEXEME *lexeme)
{
    fprintf(fp, "%s", lexeme->type);
}
