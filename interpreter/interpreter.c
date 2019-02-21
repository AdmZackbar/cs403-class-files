#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
static LEXEME *evalEquals(LEXEME *tree, LEXEME *env);
static LEXEME *evalPlus(LEXEME *tree, LEXEME *env);
static LEXEME *evalMinus(LEXEME *tree, LEXEME *env);
static LEXEME *evalTimes(LEXEME *tree, LEXEME *env);
static LEXEME *evalDivide(LEXEME *tree, LEXEME *env);
static LEXEME *evalModulus(LEXEME *tree, LEXEME *env);
static LEXEME *evalExponent(LEXEME *tree, LEXEME *env);
static LEXEME *evalEqualsEquals(LEXEME *tree, LEXEME *env);
static LEXEME *evalNotEquals(LEXEME *tree, LEXEME *env);
static LEXEME *evalLessThan(LEXEME *tree, LEXEME *env);
static LEXEME *evalGreaterThan(LEXEME *tree, LEXEME *env);
static LEXEME *evalLessTE(LEXEME *tree, LEXEME *env);
static LEXEME *evalGreaterTE(LEXEME *tree, LEXEME *env);
static LEXEME *evalLogicalAnd(LEXEME *tree, LEXEME *env);
static LEXEME *evalLogicalOr(LEXEME *tree, LEXEME *env);
static LEXEME *evalBinaryAnd(LEXEME *tree, LEXEME *env);
static LEXEME *evalBinaryOr(LEXEME *tree, LEXEME *env);
static LEXEME *evalDot(LEXEME *tree, LEXEME *env);
static LEXEME *evalParen(LEXEME *tree, LEXEME *env);
static LEXEME *evalUnaryID(LEXEME *tree, LEXEME *env);
static LEXEME *evalFunctionCall(LEXEME *tree, LEXEME *env);
static LEXEME *evalConstructor(LEXEME *tree, LEXEME *env);
static LEXEME *evalBuiltIn(LEXEME *tree, LEXEME *env);
static LEXEME *evalArgs(LEXEME *args, LEXEME *env);
static LEXEME *evalStatements(LEXEME *tree, LEXEME *env);

static void failExpr(char *expected, char *exprType, LEXEME *badLex);

LEXEME *mainFunction = NULL;
LEXEME *mainArgs = NULL;

int main(int argc, char **argv)
{
    char *filename = parseFileArg(argc, argv);

    LEXEME *globalEnv = newEnvironment();
    LEXEME *rootLex = parse(filename);

    eval(rootLex, globalEnv);

    LEXEME *returnVal;
    if (mainFunction)   returnVal = eval(mainFunction, globalEnv);
    else
    {
        fprintf(stderr, "No main function located\n");
        return -1;
    }
    
    return getIntLEXEME(returnVal);
}

static char *parseFileArg(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Incorrect number of parameters, expected 2 or more, got %d.\n", argc);
        printf("Correct usage: %s <filepath> [args+]\n", argv[0]);
        exit(-1);
    }
    
    LEXEME *arg;
    for (int i=argc-1; i>=2; i--)
    {
        arg = newLEXEMEstring(argv[i], -1);
        mainArgs = cons(VAR_LIST, arg, mainArgs);
    }
    
    return argv[1];
}

static LEXEME *eval(LEXEME *tree, LEXEME *env)
{
    char *type = getTypeLEXEME(tree);
    if (isPrimative(tree) || type == ID || type == ARRAY_LOOKUP)  return tree;    // Int, real, or str
    if (type == ID) return getValueEnv(env, tree);
    if (type == PROG)   return evalProgram(tree, env);
    if (type == CLASS_DEF)  return evalClassDef(tree, env);
    if (type == CLASS_HEADER)   return evalClassHeader(tree, env);
    if (type == CLASS_STATEMENTS)   return evalClassStatements(tree, env);
    if (type == CLASS_STATEMENT)    return evalClassStatement(tree, env);
    if (type == VAR_DECL)   return evalVarDecl(tree, env);
    if (type == FUNCTION_STATEMENT) return evalFunctionStatement(tree, env);
    if (type == EQUALS) return evalEquals(tree, env);
    if (type == PLUS)   return evalPlus(tree, env);
    if (type == MINUS)  return evalMinus(tree, env);
    if (type == TIMES)  return evalTimes(tree, env);
    if (type == DIVIDE) return evalDivide(tree, env);
    if (type == MODULUS)    return evalModulus(tree, env);
    if (type == EXPONENT)   return evalExponent(tree, env);
    if (type == EQUALSEQUALS)   return evalEqualsEquals(tree, env);
    if (type == NOTEQUALS)  return evalNotEquals(tree, env);
    if (type == LESS_THAN)  return evalLessThan(tree, env);
    if (type == GREATER_THAN)   return evalGreaterThan(tree, env);
    if (type == LESS_THAN_EQUAL)    return evalLessTE(tree, env);
    if (type == GREATER_THAN_EQUAL)    return evalGreaterTE(tree, env);
    if (type == LOGICAL_AND)    return evalLogicalAnd(tree, env);
    if (type == LOGICAL_OR) return evalLogicalOr(tree, env);
    if (type == BINARY_AND) return evalBinaryAnd(tree, env);
    if (type == BINARY_OR)  return evalBinaryOr(tree, env);
    if (type == DOT)    return evalDot(tree, env);
    if (type == UNARY_PAREN)    return evalParen(tree, env);
    if (type == UNARY_ID)   return evalUnaryID(tree, env);
    if (type == FUNCTION_CALL)  return evalFunctionCall(tree, env);
    if (type == EXPR_LIST)  return evalArgs(tree, env);
}

static LEXEME *evalProgram(LEXEME *tree, LEXEME *env)
{
    LEXEME *returnVal = eval(car(tree), env);   // ClassDef
    if(cdr(tree))   return eval(cdr(tree), env);    // Program
    return returnVal;
}

static LEXEME *evalClassDef(LEXEME *tree, LEXEME *env)
{
    LEXEME *closure = eval(car(tree), env);    // ClassHeader
    if (cdr(tree))  eval(cdr(tree), env);  // ClassStatements
    //insertEnvironment(env, newLEXEMEstring("this", -1), closure);
    return closure;
}

static LEXEME *evalClassHeader(LEXEME *tree, LEXEME *env)
{
    if (cdr(tree))
    {
        // If we need to extend from a previous class - TODO
    }
    LEXEME *closure = cons(OCLOSURE, env, tree);
    insertEnvironment(env, car(tree), closure);
    return closure;
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
    return eval(cdr(tree), env);    // FunctionStatement or VarDecl
}

static LEXEME *evalVarDecl(LEXEME *tree, LEXEME *env)
{
    LEXEME *varDef, *value = NULL;
    tree = car(tree);   // VarList
    while (tree)
    {
        varDef = car(tree);
        if (cdr(varDef))
            value = eval(cdr(varDef), env); // Expr
        insertEnvironment(env, car(varDef), value);  // ID - var name
        tree = cdr(tree);
    }
    return env;
}

static LEXEME *evalFunctionStatement(LEXEME *tree, LEXEME *env)
{
    LEXEME *closure = cons(CLOSURE, env, tree);
    
    if (strcmp(getStrLEXEME(car(cdr(tree))), "main"))    // if function name is main
    {
        if (mainFunction == NULL)   mainFunction = closure; // TODO - handle params
        else
        {
            fprintf(stderr, "Additional main function found, exiting...\n");
            exit(-400);
        }
    }
    else
    {
        insertEnvironment(env, car(cdr(tree)), closure);
    }
    
    return env;
}

static LEXEME *evalEquals(LEXEME *tree, LEXEME *env)
{
    LEXEME *left = eval(car(tree), env);
    if (getTypeLEXEME(left) != ID)  failExpr("ID", "equals", left);
    LEXEME *right = eval(cdr(tree), env);
    setValueEnv(env, left, right);
    return right;
}

static LEXEME *evalPlus(LEXEME *tree, LEXEME *env)
{
    LEXEME *left = eval(car(tree), env);
    LEXEME *right = eval(cdr(tree), env);
    char *leftType = getTypeLEXEME(left), *rightType = getTypeLEXEME(right);
    if (leftType == INTEGER)
    {
        if (rightType == INTEGER)   return newLEXEMEint(getIntLEXEME(left) + getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEreal(getIntLEXEME(left) + getRealLEXEME(right), -1);
        //if (rightType == STRING)    return newLEXEMEstring
        failExpr("INTEGER or REAL", "plus", right);
    }
    if (leftType == REAL)
    {
        if (rightType == INTEGER)   return newLEXEMEreal(getRealLEXEME(left) + getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEreal(getRealLEXEME(left) + getRealLEXEME(right), -1);
        //if (rightType == STRING)    return newLEXEMEstring
        failExpr("INTEGER or REAL", "plus", right);
    }
    failExpr("INTEGER or REAL", "plus", left);
    return NULL;    // Unreachable - for compiler
}

static LEXEME *evalMinus(LEXEME *tree, LEXEME *env)
{
    LEXEME *left = eval(car(tree), env);
    LEXEME *right = eval(cdr(tree), env);
    char *leftType = getTypeLEXEME(left), *rightType = getTypeLEXEME(right);
    if (leftType == INTEGER)
    {
        if (rightType == INTEGER)   return newLEXEMEint(getIntLEXEME(left) - getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEreal(getIntLEXEME(left) - getRealLEXEME(right), -1);
        //if (rightType == STRING)    return newLEXEMEstring
        failExpr("INTEGER or REAL", "minus", right);
    }
    if (leftType == REAL)
    {
        if (rightType == INTEGER)   return newLEXEMEreal(getRealLEXEME(left) - getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEreal(getRealLEXEME(left) - getRealLEXEME(right), -1);
        //if (rightType == STRING)    return newLEXEMEstring
        failExpr("INTEGER or REAL", "minus", right);
    }
    failExpr("INTEGER or REAL", "minus", left);
    return NULL;    // Unreachable - for compiler
}

static LEXEME *evalTimes(LEXEME *tree, LEXEME *env)
{
    LEXEME *left = eval(car(tree), env);
    LEXEME *right = eval(cdr(tree), env);
    char *leftType = getTypeLEXEME(left), *rightType = getTypeLEXEME(right);
    if (leftType == INTEGER)
    {
        if (rightType == INTEGER)   return newLEXEMEint(getIntLEXEME(left) * getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEreal(getIntLEXEME(left) * getRealLEXEME(right), -1);
        failExpr("INTEGER or REAL", "times", right);
    }
    if (leftType == REAL)
    {
        if (rightType == INTEGER)   return newLEXEMEreal(getRealLEXEME(left) * getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEreal(getRealLEXEME(left) * getRealLEXEME(right), -1);
        failExpr("INTEGER or REAL", "times", right);
    }
    failExpr("INTEGER or REAL", "times", left);
    return NULL;    // Unreachable - for compiler
}

static LEXEME *evalDivide(LEXEME *tree, LEXEME *env)
{
    LEXEME *left = eval(car(tree), env);
    LEXEME *right = eval(cdr(tree), env);
    char *leftType = getTypeLEXEME(left), *rightType = getTypeLEXEME(right);
    if (leftType == INTEGER)
    {
        if (rightType == INTEGER)   return newLEXEMEint(getIntLEXEME(left) / getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEreal(getIntLEXEME(left) / getRealLEXEME(right), -1);
        failExpr("INTEGER or REAL", "divide", right);
    }
    if (leftType == REAL)
    {
        if (rightType == INTEGER)   return newLEXEMEreal(getRealLEXEME(left) / getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEreal(getRealLEXEME(left) / getRealLEXEME(right), -1);
        failExpr("INTEGER or REAL", "divide", right);
    }
    failExpr("INTEGER or REAL", "divide", left);
    return NULL;    // Unreachable - for compiler
}

static LEXEME *evalModulus(LEXEME *tree, LEXEME *env)
{
    LEXEME *left = eval(car(tree), env);
    LEXEME *right = eval(cdr(tree), env);
    char *leftType = getTypeLEXEME(left), *rightType = getTypeLEXEME(right);
    if (leftType == INTEGER)
    {
        if (rightType == INTEGER)   return newLEXEMEint(getIntLEXEME(left) % getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEreal(fmod(getIntLEXEME(left), getRealLEXEME(right)), -1);
        failExpr("INTEGER or REAL", "modulus", right);
    }
    if (leftType == REAL)
    {
        if (rightType == INTEGER)   return newLEXEMEreal(fmod(getRealLEXEME(left), getIntLEXEME(right)), -1);
        if (rightType == REAL)  return newLEXEMEreal(fmod(getRealLEXEME(left), getRealLEXEME(right)), -1);
        failExpr("INTEGER or REAL", "modulus", right);
    }
    failExpr("INTEGER or REAL", "modulus", left);
    return NULL;    // Unreachable - for compiler
}

static LEXEME *evalExponent(LEXEME *tree, LEXEME *env)
{
    LEXEME *left = eval(car(tree), env);
    LEXEME *right = eval(cdr(tree), env);
    char *leftType = getTypeLEXEME(left), *rightType = getTypeLEXEME(right);
    if (leftType == INTEGER)
    {
        if (rightType == INTEGER)   return newLEXEMEreal(pow(getIntLEXEME(left), getIntLEXEME(right)), -1);
        if (rightType == REAL)  return newLEXEMEreal(pow(getIntLEXEME(left), getRealLEXEME(right)), -1);
        failExpr("INTEGER or REAL", "exponent", right);
    }
    if (leftType == REAL)
    {
        if (rightType == INTEGER)   return newLEXEMEreal(pow(getRealLEXEME(left), getIntLEXEME(right)), -1);
        if (rightType == REAL)  return newLEXEMEreal(pow(getRealLEXEME(left), getRealLEXEME(right)), -1);
        failExpr("INTEGER or REAL", "exponent", right);
    }
    failExpr("INTEGER or REAL", "exponent", left);
    return NULL;    // Unreachable - for compiler
}

static LEXEME *evalEqualsEquals(LEXEME *tree, LEXEME *env)
{
    LEXEME *left = eval(car(tree), env);
    LEXEME *right = eval(cdr(tree), env);
    char *leftType = getTypeLEXEME(left), *rightType = getTypeLEXEME(right);
    if (leftType == INTEGER)
    {
        if (rightType == INTEGER)   return newLEXEMEint(getIntLEXEME(left) == getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEint(getIntLEXEME(left) == getRealLEXEME(right), -1);
        //if (rightType == STRING)    return newLEXEMEstring
        failExpr("INTEGER or REAL", "==", right);
    }
    if (leftType == REAL)
    {
        if (rightType == INTEGER)   return newLEXEMEint(getRealLEXEME(left) == getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEint(getRealLEXEME(left) == getRealLEXEME(right), -1);
        //if (rightType == STRING)    return newLEXEMEstring
        failExpr("INTEGER or REAL", "==", right);
    }
    failExpr("INTEGER or REAL", "==", left);
    return NULL;    // Unreachable - for compiler
}

static LEXEME *evalNotEquals(LEXEME *tree, LEXEME *env)
{
    LEXEME *left = eval(car(tree), env);
    LEXEME *right = eval(cdr(tree), env);
    char *leftType = getTypeLEXEME(left), *rightType = getTypeLEXEME(right);
    if (leftType == INTEGER)
    {
        if (rightType == INTEGER)   return newLEXEMEint(getIntLEXEME(left) != getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEint(getIntLEXEME(left) != getRealLEXEME(right), -1);
        //if (rightType == STRING)    return newLEXEMEstring
        failExpr("INTEGER or REAL", "!=", right);
    }
    if (leftType == REAL)
    {
        if (rightType == INTEGER)   return newLEXEMEint(getRealLEXEME(left) != getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEint(getRealLEXEME(left) != getRealLEXEME(right), -1);
        //if (rightType == STRING)    return newLEXEMEstring
        failExpr("INTEGER or REAL", "!=", right);
    }
    failExpr("INTEGER or REAL", "!=", left);
    return NULL;    // Unreachable - for compiler
}

static LEXEME *evalLessThan(LEXEME *tree, LEXEME *env)
{
    LEXEME *left = eval(car(tree), env);
    LEXEME *right = eval(cdr(tree), env);
    char *leftType = getTypeLEXEME(left), *rightType = getTypeLEXEME(right);
    if (leftType == INTEGER)
    {
        if (rightType == INTEGER)   return newLEXEMEint(getIntLEXEME(left) < getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEint(getIntLEXEME(left) < getRealLEXEME(right), -1);
        //if (rightType == STRING)    return newLEXEMEstring
        failExpr("INTEGER or REAL", "<", right);
    }
    if (leftType == REAL)
    {
        if (rightType == INTEGER)   return newLEXEMEint(getRealLEXEME(left) < getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEint(getRealLEXEME(left) < getRealLEXEME(right), -1);
        //if (rightType == STRING)    return newLEXEMEstring
        failExpr("INTEGER or REAL", "<", right);
    }
    failExpr("INTEGER or REAL", "<", left);
    return NULL;    // Unreachable - for compiler    
}

static LEXEME *evalGreaterThan(LEXEME *tree, LEXEME *env)
{
    LEXEME *left = eval(car(tree), env);
    LEXEME *right = eval(cdr(tree), env);
    char *leftType = getTypeLEXEME(left), *rightType = getTypeLEXEME(right);
    if (leftType == INTEGER)
    {
        if (rightType == INTEGER)   return newLEXEMEint(getIntLEXEME(left) > getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEint(getIntLEXEME(left) > getRealLEXEME(right), -1);
        //if (rightType == STRING)    return newLEXEMEstring
        failExpr("INTEGER or REAL", ">", right);
    }
    if (leftType == REAL)
    {
        if (rightType == INTEGER)   return newLEXEMEint(getRealLEXEME(left) > getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEint(getRealLEXEME(left) > getRealLEXEME(right), -1);
        //if (rightType == STRING)    return newLEXEMEstring
        failExpr("INTEGER or REAL", ">", right);
    }
    failExpr("INTEGER or REAL", ">", left);
    return NULL;    // Unreachable - for compiler
}

static LEXEME *evalLessTE(LEXEME *tree, LEXEME *env)
{
    LEXEME *left = eval(car(tree), env);
    LEXEME *right = eval(cdr(tree), env);
    char *leftType = getTypeLEXEME(left), *rightType = getTypeLEXEME(right);
    if (leftType == INTEGER)
    {
        if (rightType == INTEGER)   return newLEXEMEint(getIntLEXEME(left) <= getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEint(getIntLEXEME(left) <= getRealLEXEME(right), -1);
        //if (rightType == STRING)    return newLEXEMEstring
        failExpr("INTEGER or REAL", "<=", right);
    }
    if (leftType == REAL)
    {
        if (rightType == INTEGER)   return newLEXEMEint(getRealLEXEME(left) <= getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEint(getRealLEXEME(left) <= getRealLEXEME(right), -1);
        //if (rightType == STRING)    return newLEXEMEstring
        failExpr("INTEGER or REAL", "<=", right);
    }
    failExpr("INTEGER or REAL", "<=", left);
    return NULL;    // Unreachable - for compiler
}

static LEXEME *evalGreaterTE(LEXEME *tree, LEXEME *env)
{
    LEXEME *left = eval(car(tree), env);
    LEXEME *right = eval(cdr(tree), env);
    char *leftType = getTypeLEXEME(left), *rightType = getTypeLEXEME(right);
    if (leftType == INTEGER)
    {
        if (rightType == INTEGER)   return newLEXEMEint(getIntLEXEME(left) >= getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEint(getIntLEXEME(left) >= getRealLEXEME(right), -1);
        //if (rightType == STRING)    return newLEXEMEstring
        failExpr("INTEGER or REAL", ">=", right);
    }
    if (leftType == REAL)
    {
        if (rightType == INTEGER)   return newLEXEMEint(getRealLEXEME(left) >= getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEint(getRealLEXEME(left) >= getRealLEXEME(right), -1);
        //if (rightType == STRING)    return newLEXEMEstring
        failExpr("INTEGER or REAL", ">=", right);
    }
    failExpr("INTEGER or REAL", ">=", left);
    return NULL;    // Unreachable - for compiler
}

static LEXEME *evalLogicalAnd(LEXEME *tree, LEXEME *env)
{
    LEXEME *left = eval(car(tree), env);
    LEXEME *right = eval(cdr(tree), env);
    char *leftType = getTypeLEXEME(left), *rightType = getTypeLEXEME(right);
    if (leftType == INTEGER)
    {
        if (rightType == INTEGER)   return newLEXEMEint(getIntLEXEME(left) && getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEint(getIntLEXEME(left) && getRealLEXEME(right), -1);
        failExpr("INTEGER or REAL", "&&", right);
    }
    if (leftType == REAL)
    {
        if (rightType == INTEGER)   return newLEXEMEint(getRealLEXEME(left) && getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEint(getRealLEXEME(left) && getRealLEXEME(right), -1);
        failExpr("INTEGER or REAL", "&&", right);
    }
    failExpr("INTEGER or REAL", "&&", left);
    return NULL;    // Unreachable - for compiler
}

static LEXEME *evalLogicalOr(LEXEME *tree, LEXEME *env)
{
    LEXEME *left = eval(car(tree), env);
    LEXEME *right = eval(cdr(tree), env);
    char *leftType = getTypeLEXEME(left), *rightType = getTypeLEXEME(right);
    if (leftType == INTEGER)
    {
        if (rightType == INTEGER)   return newLEXEMEint(getIntLEXEME(left) || getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEint(getIntLEXEME(left) || getRealLEXEME(right), -1);
        failExpr("INTEGER or REAL", "||", right);
    }
    if (leftType == REAL)
    {
        if (rightType == INTEGER)   return newLEXEMEint(getRealLEXEME(left) || getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEint(getRealLEXEME(left) || getRealLEXEME(right), -1);
        failExpr("INTEGER or REAL", "||", right);
    }
    failExpr("INTEGER or REAL", "||", left);
    return NULL;    // Unreachable - for compiler
}

static LEXEME *evalBinaryAnd(LEXEME *tree, LEXEME *env)
{
    LEXEME *left = eval(car(tree), env);
    LEXEME *right = eval(cdr(tree), env);
    char *leftType = getTypeLEXEME(left), *rightType = getTypeLEXEME(right);
    if (leftType == INTEGER)
    {
        if (rightType == INTEGER)   return newLEXEMEint(getIntLEXEME(left) & getIntLEXEME(right), -1);
        failExpr("INTEGER", "&", right);
    }
    failExpr("INTEGER", "&", left);
    return NULL;    // Unreachable - for compiler
}

static LEXEME *evalBinaryOr(LEXEME *tree, LEXEME *env)
{
    LEXEME *left = eval(car(tree), env);
    LEXEME *right = eval(cdr(tree), env);
    char *leftType = getTypeLEXEME(left), *rightType = getTypeLEXEME(right);
    if (leftType == INTEGER)
    {
        if (rightType == INTEGER)   return newLEXEMEint(getIntLEXEME(left) | getIntLEXEME(right), -1);
        failExpr("INTEGER", "|", right);
    }
    failExpr("INTEGER", "|", left);
    return NULL;    // Unreachable - for compiler
}

static LEXEME *evalDot(LEXEME *tree, LEXEME *env)
{
    LEXEME *object = eval(car(tree), env);
    LEXEME *field = eval(cdr(tree), env);
    return getValueEnv(car(getValueEnv(env, object)), field);
}

static LEXEME *evalParen(LEXEME *tree, LEXEME *env)
{
    return eval(car(tree), env);
}

static LEXEME *evalUnaryID(LEXEME *tree, LEXEME *env)
{
    LEXEME *var = eval(car(tree), env); // IDExpr
    LEXEME *value = getValueEnv(env, var);
    LEXEME *postVar = cdr(tree);    // PostVar
    if (postVar)
    {
        int shift = 0;
        char *valType = getTypeLEXEME(value);
        if (getTypeLEXEME(postVar) == PLUSPLUS)    shift = 1;
        if (getTypeLEXEME(postVar) == MINUSMINUS)  shift = -1;
        if (valType == INTEGER)
        {
            setValueEnv(env, var, newLEXEMEint(getIntLEXEME(value)+shift, -1));
            return value;
        }
        if (valType == REAL)
        {
            setValueEnv(env, var, newLEXEMEreal(getRealLEXEME(value)+shift, -1));
            return value;
        }
    }
    return value;
}

static LEXEME *evalFunctionCall(LEXEME *tree, LEXEME *env)
{
    LEXEME *closure = getValueEnv(env, car(tree));
    char *closureType = getTypeLEXEME(closure);
    LEXEME *args = evalArgs(cdr(tree), env);
    if (closureType == BUILT_IN)    return evalBuiltIn(closure, args);
    if (closureType == OCLOSURE)    return evalConstructor(closure, env);
    if (closureType == CLOSURE)
    {
        LEXEME *staticEnv = car(closure);
        LEXEME *params = car(cdr(cdr(closure)));
        // TODO - add support for opt args
        LEXEME *localEnv = newScopeEnv(env, params, args);
        LEXEME *body = cdr(cdr(closure));
        LEXEME *result = evalStatements(body, localEnv);
        if (getTypeLEXEME(result) == RETURNED)  return car(result);
        return result;
    }
    fprintf(stderr, "Invalid closure type given in function call\n");
    exit(-210);
}

static LEXEME *evalConstructor(LEXEME *closure, LEXEME *env)
{
    LEXEME *staticEnv = car(closure);
    LEXEME *extendedEnv = newScopeEnv(staticEnv, NULL, NULL);
    LEXEME *body = cdr(cdr(closure));
    evalStatements(body, extendedEnv);
    return extendedEnv;
}

static LEXEME *evalBuiltIn(LEXEME *tree, LEXEME *env)
{
    // TODO
    return NULL;
}

static LEXEME *evalArgs(LEXEME *args, LEXEME *env)
{
    if (args == NULL)   return NULL;
    return cons(EVAL_EXPR, eval(car(args), env), eval(cdr(args), env));
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

static void failExpr(char *expected, char *exprType, LEXEME *badLex)
{
    fprintf(stderr, "Expected type %s in %s expression. Got %s", expected, exprType, getTypeLEXEME(badLex));
    exit(-101);
}
