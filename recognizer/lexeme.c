#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "lexeme.h"
#include "types.h"

/*
 * Author:  Zach Wassynger
 * Date:    1/20/2019
 * Purpose: Part of the designer programming language project of CS 403.
 *          Holds information about certain types of grammar objects.
 */

struct lexeme
{
    char *type;             // Stores a constant string(from types)
    int lineNum;            // Stores the line number where the lexeme was found
    int iVal;               // Holds integer values
    double rVal;            // Holds real values
    char *sVal;             // Holds string values
    LEXEME *left, *right;   // Points to the children of the lexeme
};

LEXEME *newLEXEME(char *type, int lineNum)
{
    LEXEME *l = malloc(sizeof(LEXEME));
    assert(l != 0);

    l->type = type;
    l->lineNum = lineNum;
    l->sVal = NULL;
    l->left = NULL;
    l->right = NULL;
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

extern LEXEME *getLEXEMEleft(LEXEME *lexeme)
{
    assert(lexeme != 0);
    return lexeme->left;
}
extern LEXEME *getLEXEMEright(LEXEME *lexeme)
{
    assert(lexeme != 0);
    return lexeme->right;
}

extern void setLEXEMEleft(LEXEME *parent, LEXEME *child)
{
    assert(parent != 0);
    parent->left = child;
}
extern void setLEXEMEright(LEXEME *parent, LEXEME *child)
{
    assert(parent != 0);
    parent->right = child;
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
    if (lexeme->type == INTEGER)
        fprintf(fp, " - %d", lexeme->iVal);
    else if (lexeme->type == REAL)
        fprintf(fp, " - %f", lexeme->rVal);
    else if (lexeme->sVal != NULL)
        fprintf(fp, " - %s", lexeme->sVal);
}
