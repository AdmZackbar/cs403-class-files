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

static void failExpr(char *msg, LEXEME *badLex);

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
    if (type == EQUALS) return evalEquals(tree, env);
    if (type == PLUS)   return evalPlus(tree, env);
    if (type == MINUS)  return evalMinus(tree, env);
    if (type == TIMES)  return evalTimes(tree, env);
    if (type == DIVIDE) return evalDivide(tree, env);
    if (type == MODULUS)    return evalModulus(tree, env);
    if (type == EXPONANT)   return evalExponent(tree, env);
    if (type == EQUALSEQUALS)   return evalEqualsEquals(tree, env);
    if (type == NOTEQUALS)  return evalNotEquals(tree, env);
    if (type == LESS_THAN)  return evalLessThan(tree, env);
    if (type == GREATER_THAN)   return evalGreaterThan(tree, env);
    if (type == LESS_THAN_EQUALS)   return evalLessTE(tree, env);
    if (type == GREATER_THAN_EQUALS)    return evalGreaterTE(tree, env);
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
        if (rightType == REAL)  return newLEXEMEdouble(getIntLEXEME(left) + getRealLEXEME(right), -1);
        //if (rightType == STRING)    return newLEXEMEstring
        failExpr("INTEGER or REAL", "plus", right);
    }
    if (leftType == REAL)
    {
        if (rightType == INTEGER)   return newLEXEMEdouble(getRealLEXEME(left) + getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEdouble(getRealLEXEME(left) + getRealLEXEME(right), -1);
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
        if (rightType == REAL)  return newLEXEMEdouble(getIntLEXEME(left) - getRealLEXEME(right), -1);
        //if (rightType == STRING)    return newLEXEMEstring
        failExpr("INTEGER or REAL", "minus", right);
    }
    if (leftType == REAL)
    {
        if (rightType == INTEGER)   return newLEXEMEdouble(getRealLEXEME(left) - getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEdouble(getRealLEXEME(left) - getRealLEXEME(right), -1);
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
        if (rightType == REAL)  return newLEXEMEdouble(getIntLEXEME(left) * getRealLEXEME(right), -1);
        failExpr("INTEGER or REAL", "times", right);
    }
    if (leftType == REAL)
    {
        if (rightType == INTEGER)   return newLEXEMEdouble(getRealLEXEME(left) * getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEdouble(getRealLEXEME(left) * getRealLEXEME(right), -1);
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
        if (rightType == REAL)  return newLEXEMEdouble(getIntLEXEME(left) / getRealLEXEME(right), -1);
        failExpr("INTEGER or REAL", "divide", right);
    }
    if (leftType == REAL)
    {
        if (rightType == INTEGER)   return newLEXEMEdouble(getRealLEXEME(left) / getIntLEXEME(right), -1);
        if (rightType == REAL)  return newLEXEMEdouble(getRealLEXEME(left) / getRealLEXEME(right), -1);
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
        if (rightType == REAL)  return newLEXEMEdouble(fmod(getIntLEXEME(left), getRealLEXEME(right)), -1);
        failExpr("INTEGER or REAL", "modulus", right);
    }
    if (leftType == REAL)
    {
        if (rightType == INTEGER)   return newLEXEMEdouble(fmod(getRealLEXEME(left), getIntLEXEME(right)), -1);
        if (rightType == REAL)  return newLEXEMEdouble(fmod(getRealLEXEME(left), getRealLEXEME(right)), -1);
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
        if (rightType == INTEGER)   return newLEXEMEdouble(pow(getIntLEXEME(left), getIntLEXEME(right)), -1);
        if (rightType == REAL)  return newLEXEMEdouble(pow(getIntLEXEME(left), getRealLEXEME(right)), -1);
        failExpr("INTEGER or REAL", "exponent", right);
    }
    if (leftType == REAL)
    {
        if (rightType == INTEGER)   return newLEXEMEdouble(pow(getRealLEXEME(left), getIntLEXEME(right)), -1);
        if (rightType == REAL)  return newLEXEMEdouble(pow(getRealLEXEME(left), getRealLEXEME(right)), -1);
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
