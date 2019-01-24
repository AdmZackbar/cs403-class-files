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

LEXEME *newLEXEME(char *type, int lineNum)
{
    LEXEME *l = malloc(sizeof(LEXEME));
    assert(l != 0);

    l->type = type;
    l->lineNum = lineNum;
    l->sVal = NULL;
    return l;
}

LEXEME *newLEXEMEstring(char *type, char *value, int lineNum)
{
    LEXEME *l = newLEXEME(type, lineNum);
    l->sVal = value;
    return l;
}

LEXEME *newLEXEMEdouble(double value, int lineNum)
{
    LEXEME *l = newLEXEME(REAL, lineNum);
    l->rVal = value;
    return l;
}
LEXEME *newLEXEMEint(int value, int lineNum)
{
    LEXEME *l = newLEXEME(INTEGER, lineNum);
    l->iVal = value;
    return l;
}

char *getTypeLEXEME(LEXEME *lexeme)
{
    return lexeme->type;
}

int getLineNumLEXEME(LEXEME *lexeme)
{
    return lexeme->lineNum;
}

int isErrorLEXEME(LEXEME *lexeme)
{
    return lexeme->type == PARSE_ERROR || lexeme->type == BAD_NUM;
}

void printLEXEME(FILE *fp, LEXEME *lexeme)
{
    fprintf(fp, "%s - line %d", lexeme->type, lexeme->lineNum);
}
