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
static LEXEME *evalClassHeader(LEXEME *tree, LEXEME *env);
static LEXEME *evalClassStatements(LEXEME *tree, LEXEME *env);
static LEXEME *evalClassStatement(LEXEME *tree, LEXEME *env);
static LEXEME *evalVarDecl(LEXEME *tree, LEXEME *env);
static LEXEME *evalFunctionStatement(LEXEME *tree, LEXEME *env);

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
    if (type == CLASS_HEADER)   return evalClassHeader(tree, env);
    if (type == CLASS_STATEMENTS)   return evalClassStatements(tree, env);
    if (type == CLASS_STATEMENT)    return evalClassStatement(tree, env);
    if (type == VAR_DECL)   return evalVarDecl(tree, env);
    if (type == FUNCTION_STATEMENT) return evalFunctionStatement(tree, env);
}

static LEXEME *evalProgram(LEXEME *tree, LEXEME *env)
{
    LEXEME *returnVal = eval(car(tree), env);   // ClassDef
    if(cdr(tree))   return eval(cdr(tree), env);    // Program
    return returnVal;
}

static LEXEME *evalClassDef(LEXEME *tree, LEXEME *env)
{
    LEXEME *classEnv = eval(car(tree), env);    // ClassHeader
    if (cdr(tree))  eval(cdr(tree), classEnv);  // ClassStatements
}

static LEXEME *evalClassHeader(LEXEME *tree, LEXEME *env)
{
    if (cdr(tree))
    {
        // If we need to extend from a previous class - TODO
    }
    insertEnvironment(env, car(tree), cons(OCLOSURE, env, tree));
    return env;
}

static LEXEME *evalClassStatements(LEXEME *tree, LEXEME *env)
{
    LEXEME *returnedVal;
    while (tree)
    {
        returnedVal = eval(car(tree), env); // ClassStatement
        tree = cdr(tree);   // ClassStatements
    }
    return returnedVal;
}

static LEXEME *evalClassStatement(LEXEME *tree, LEXEME *env)
{
    // N.B. Currently access mods are not implemented
    return eval(cdr(tree), env);    // FunctionStatement or VarDecl - TODO
}

static LEXEME *evalVarDecl(LEXEME *tree, LEXEME *env)
{
    LEXEME *varDef, *value = NULL;
    tree = car(tree);   // VarList
    while (tree)
    {
        varDef = car(tree);
        if (cdr(varDef))
            value = eval(cdr(varDef), env); // Expr - TODO
        insertEnvironment(env, car(varDef), value);  // ID - var name
        tree = cdr(tree);
    }
    return env;
}

static LEXEME *evalFunctionStatement(LEXEME *tree, LEXEME *env)
{
    insertEnvironment(env, car(cdr(tree)), cons(CLOSURE, env, tree));   // ID - function name
    return env;
}

static LEXEME *evalStatements(LEXEME *tree, LEXEME *env)
{
    LEXEME *result;
    while (tree)
    {
        result = eval(car(tree), env);  // Statement - if, else, etc - TODO
        if (getTypeLEXEME(result) == RETURNED)
            break;
        tree = cdr(tree);
    }
    return result;
}
