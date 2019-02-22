#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
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
    char *type;                 // Stores a constant string(from types)
    int lineNum;                // Stores the line number where the lexeme was found
    int iVal;                   // Holds integer values
    double rVal;                // Holds real values
    char *sVal;                 // Holds string values
    LEXEME **aVal;              // Holds array values
    LEXEME *(*fVal)(LEXEME *);  // Holds a function
    LEXEME *left, *right;       // Points to the children of the lexeme
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

LEXEME *newLEXEMEreal(double value, int lineNum)
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
LEXEME *newLEXEMEarray(int size)
{
    LEXEME *l = newLEXEME(ARRAY, -1);
    l->aVal = malloc(sizeof(LEXEME *) * size);
    l->iVal = size;
    assert(l->aVal != 0);
    return l;
}
LEXEME *newLEXEMEfunction(char *type, LEXEME *(*fVal)(LEXEME *))
{
    LEXEME *l = newLEXEME(type, -1);
    l->fVal = fVal;
    return l;
}

LEXEME *cons(char *type, LEXEME *left, LEXEME *right)
{
    LEXEME *parent = newLEXEME(type, -1);
    parent->left = left;
    parent->right = right;

    return parent;
}

LEXEME *car(LEXEME *lexeme)
{
    assert(lexeme != 0);
    return lexeme->left;
}
LEXEME *cdr(LEXEME *lexeme)
{
    assert(lexeme != 0);
    return lexeme->right;
}

void setCar(LEXEME *parent, LEXEME *child)
{
    assert(parent != 0);
    parent->left = child;
}
void setCdr(LEXEME *parent, LEXEME *child)
{
    assert(parent != 0);
    parent->right = child;
}

LEXEME *setArrayValueLEXEME(LEXEME *array, int index, LEXEME *value)
{
    assert(array != 0);
    if (index < 0 || index >= array->iVal)
    {
        fprintf(stderr, "Array out of bounds exception. Array size: %d, index given: %d\n", array->iVal, index);
        exit(-101);
    }
    LEXEME *oldVal = array->aVal[index];
    array->aVal[index] = value;
    return oldVal;
}

char *getTypeLEXEME(LEXEME *lexeme)
{
    assert(lexeme != 0);
    return lexeme->type;
}

int getLineNumLEXEME(LEXEME *lexeme)
{
    assert(lexeme != 0);
    return lexeme->lineNum;
}

int getIntLEXEME(LEXEME *lexeme)
{
    assert(lexeme != 0);
    return lexeme->iVal;
}

double getRealLEXEME(LEXEME *lexeme)
{
    assert(lexeme != 0);
    return lexeme->rVal;
}

char *getStrLEXEME(LEXEME *lexeme)
{
    assert(lexeme != 0);
    return lexeme->sVal;
}

LEXEME *getArrayValueLEXEME(LEXEME *array, int index)
{
    assert(array != 0);
    if (index < 0 || index >= array->iVal)
    {
        fprintf(stderr, "Array out of bounds exception. Array size: %d, index given: %d\n", array->iVal, index);
        exit(-101);
    }
    return array->aVal[index];
}

LEXEME *evalFunctionLEXEME(LEXEME *lexeme, LEXEME *args)
{
    return lexeme->fVal(args);
}

int sameVar(LEXEME *var1, LEXEME *var2)
{
    //printf("var1(%s): %s\tvar2(%s): %s\n", getTypeLEXEME(var1), getStrLEXEME(var1), getTypeLEXEME(var2), getStrLEXEME(var2));
    assert(var1 != 0 && var2 != 0 && var1->sVal != 0 && var2->sVal != 0);
    return !strcmp(var1->sVal, var2->sVal);
}

int isErrorLEXEME(LEXEME *lexeme)
{
    assert(lexeme != 0);
    return lexeme->type == PARSE_ERROR || lexeme->type == BAD_NUM;
}

int isPrimative(LEXEME *lexeme)
{
    assert(lexeme != 0);
    char *type = lexeme->type;
    return type == INTEGER || type == REAL || type == STRING;
}

int isOperator(LEXEME *lexeme)
{
    assert(lexeme != 0);
    char *type = lexeme->type;
    return type == PLUS || type == MINUS || type == TIMES || type == DIVIDE || type == MODULUS
            || type == EXPONENT || type == DOT || type == LESS_THAN || type == GREATER_THAN
            || type == LESS_THAN_EQUAL || type == GREATER_THAN_EQUAL || type == NOTEQUALS
            || type == EQUALSEQUALS || type == LOGICAL_AND || type == LOGICAL_OR || type == BINARY_AND
            || type == BINARY_OR || type == EQUALS;
}

int isUnaryOp(LEXEME *lexeme)
{
    assert(lexeme != 0);
    char *type = lexeme->type;
    return type == UMINUS || type == PLUSPLUS || type == MINUSMINUS || type == NOT;
}

int isAccessMod(LEXEME *lexeme)
{
    assert(lexeme != 0);
    char *type = lexeme->type;
    return type == PUBLIC || type == PRIVATE || type == PROTECTED;
}

void printLEXEME(FILE *fp, LEXEME *lexeme)
{
    assert(lexeme != 0);
    fprintf(fp, "%s - line %d", lexeme->type, lexeme->lineNum);
    if (lexeme->type == INTEGER)
        fprintf(fp, " - %d", lexeme->iVal);
    else if (lexeme->type == REAL)
        fprintf(fp, " - %f", lexeme->rVal);
    else if (lexeme->sVal != NULL)
        fprintf(fp, " - %s", lexeme->sVal);
}

void displayLEXEME(FILE *fp, LEXEME *lexeme)
{
    assert(lexeme != 0);
    if (lexeme->type == INTEGER)        fprintf(fp, "%d", lexeme->iVal);
    else if (lexeme->type == REAL)      fprintf(fp, "%f", lexeme->rVal);
    else if (lexeme->type == STRING)    fprintf(fp, "\"%s\"", lexeme->sVal);
    else if (lexeme->type == ID)        fprintf(fp, "%s", lexeme->sVal);
    else if (lexeme->type == EQUALS)                fprintf(fp, "=");
    else if (lexeme->type == PLUS)                  fprintf(fp, "+");
    else if (lexeme->type == MINUS)                 fprintf(fp, "-");
    else if (lexeme->type == TIMES)                 fprintf(fp, "*");
    else if (lexeme->type == DIVIDE)                fprintf(fp, "/");
    else if (lexeme->type == MODULUS)               fprintf(fp, "%%");
    else if (lexeme->type == EXPONENT)              fprintf(fp, "^");
    else if (lexeme->type == DOT)                   fprintf(fp, ".");
    else if (lexeme->type == NOT)                   fprintf(fp, "!");
    else if (lexeme->type == LESS_THAN)             fprintf(fp, "<");
    else if (lexeme->type == GREATER_THAN)          fprintf(fp, ">");
    else if (lexeme->type == LESS_THAN_EQUAL)       fprintf(fp, "<=");
    else if (lexeme->type == GREATER_THAN_EQUAL)    fprintf(fp, ">=");
    else if (lexeme->type == NOTEQUALS)             fprintf(fp, "!=");
    else if (lexeme->type == EQUALSEQUALS)          fprintf(fp, "==");
    else if (lexeme->type == LOGICAL_AND)           fprintf(fp, "&&");
    else if (lexeme->type == LOGICAL_OR)            fprintf(fp, "||");
    else if (lexeme->type == BINARY_AND)            fprintf(fp, "&");
    else if (lexeme->type == BINARY_OR)             fprintf(fp, "|");
    else if (lexeme->type == UMINUS)                fprintf(fp, "-");
    else if (lexeme->type == PLUSPLUS)              fprintf(fp, "++");
    else if (lexeme->type == MINUSMINUS)            fprintf(fp, "--");
    else if (lexeme->type == PUBLIC)    fprintf(fp, "public");
    else if (lexeme->type == PRIVATE)   fprintf(fp, "private");
    else if (lexeme->type == PROTECTED) fprintf(fp, "protected");
    else if (lexeme->type == NULL_WORD) fprintf(fp, "NULL");
    else if (lexeme->type == THIS)      fprintf(fp, "this");
}
