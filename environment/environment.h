#ifndef __ENVIRONMENT_INCLUDED__
#define __ENVIRONMENT_INCLUDED__

#include "lexeme.h"

extern LEXEME *newEnvironment();
extern LEXEME *insertEnvironment(LEXEME *env, LEXEME *id, LEXEME *value);
extern LEXEME *getValueEnv(LEXEME *env, LEXEME *id);
extern LEXEME *setValueEnv(LEXEME *env, LEXEME *id, LEXEME *newVal);
extern LEXEME *newScopeEnv(LEXEME *env, LEXEME *vars, LEXEME *vals);
extern void displayEnvironment(FILE *fp, LEXEME *env);

#endif
