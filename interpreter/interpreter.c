#include <stdio.h>
#include <stdlib.h>
#include "environment.h"
#include "lexeme.h"
#include "parser.h"
#include "types.h"

static char *parseFileArg(int argc, char **argv);
static LEXEME *eval(LEXEME *tree, LEXEME *env);
static LEXEME *evalProgram(LEXEME *tree, LEXEME *env);
static LEXEME *evalClassDef(LEXEME *tree, LEXEME *env);

int main(int argc, char **argv)
{
    char *filename = parseFileArg(argc, argv);

    LEXEME *globalEnv = newEnvironment();
    LEXEME *rootLex = parse(filename);

    LEXEME *returnVal = eval(rootLex, globalEnv);

    // TODO handle return value
    return 0;
}

static char *parseFileArg(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Incorrect number of parameters, expected 2, got %d.\n", argc);
        printf("Correct usage: %s <filepath>\n", argv[0]);
        exit(-1);
    }
    
    return argv[1];
}

static LEXEME *eval(LEXEME *tree, LEXEME *env)
{
    char *type = getTypeLEXEME(tree);
    if (isPrimative(tree))  return tree;    // Int, real, or str
    if (type == ID) return getValueEnv(env, tree);
    if (type == PROG)   return evalProgram(tree, env);
    if (type == CLASS_DEF)  return evalClassDef(tree, env);
}

static LEXEME *evalProgram(LEXEME *tree, LEXEME *env)
{
    LEXEME *returnVal = eval(car(tree), env);   // ClassDef
    if(cdr(tree))   return eval(cdr(tree), env);    // Program
    return returnVal;
}

static LEXEME *evalClassDef(LEXEME *tree, LEXEME *env)
{
    eval(car(tree), env);   // Class header - TODO
    if (cdr(tree))  eval(cdr(tree), env);   // Class statements - TODO
}
