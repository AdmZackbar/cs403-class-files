#ifndef __LEXEME_INCLUDED__
#define __LEXEME_INCLUDED__

#include <stdio.h>

typedef struct lexeme LEXEME;

extern LEXEME *newLEXEME(char *type, int lineNum);
extern LEXEME *newLEXEMEstring(char *type, char *value, int lineNum);
extern LEXEME *newLEXEMEreal(double value, int lineNum);
extern LEXEME *newLEXEMEint(int value, int lineNum);
extern LEXEME *newLEXEMEarray(int size);
extern LEXEME *newLEXEMEfunction(char *type, LEXEME *(*fVal)(LEXEME *));
extern LEXEME *newLEXEMEfile(char *filename);
extern LEXEME *cons(char *type, LEXEME *left, LEXEME *right);
extern LEXEME *car(LEXEME *lexeme);
extern LEXEME *cdr(LEXEME *lexeme);
extern void setCar(LEXEME *parent, LEXEME *child);
extern void setCdr(LEXEME *parent, LEXEME *child);
extern LEXEME *setArrayValueLEXEME(LEXEME *array, int index, LEXEME *value);
extern char *getTypeLEXEME(LEXEME *lexeme);
extern int getLineNumLEXEME(LEXEME *lexeme);
extern int getIntLEXEME(LEXEME *lexeme);
extern double getRealLEXEME(LEXEME *lexeme);
extern char *getStrLEXEME(LEXEME *lexeme);
extern LEXEME *getArrayValueLEXEME(LEXEME *array, int index);
extern LEXEME *evalFunctionLEXEME(LEXEME *lexeme, LEXEME *args);
extern int closeFileLEXEME(LEXEME *file);
extern LEXEME *readIntLEXEME(LEXEME *file);
extern int sameVar(LEXEME *var1, LEXEME *var2);
extern int isErrorLEXEME(LEXEME *lexeme);
extern int isPrimative(LEXEME *lexeme);
extern int isOperator(LEXEME *lexeme);
extern int isUnaryOp(LEXEME *lexeme);
extern int isAccessMod(LEXEME *lexeme);
extern void printLEXEME(FILE *fp, LEXEME *lexeme);
extern void displayLEXEME(FILE *fp, LEXEME *lexeme);

#endif
