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

LEXEME *newLEXEMEvalue(char *type, void *value)
{
    LEXEME *l = newLEXEME(type);
    return l;
}

LEXEME *newLEXEMEdouble(char *type, double value)
{
    LEXEME *l = newLEXEME(type);
    l->rVal = value;
    return l;
}
LEXEME *newLEXEMEint(char *type, int value)
{
    LEXEME *l = newLEXEME(type);
    l->iVal = value;
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

int isErrorLEXEME(LEXEME *lexeme)
{
    return lexeme->type == PARSE_ERROR || lexeme->type == BAD_NUM;
}

void printLEXEME(FILE *fp, LEXEME *lexeme)
{
    fprintf(fp, "%s", lexeme->type);
}
