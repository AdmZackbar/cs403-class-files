#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "environment.h"
#include "lexeme.h"
#include "parser.h"
#include "types.h"

static char *parseFileArg(int argc, char **argv);
static void addBuiltIn(LEXEME *env);
static LEXEME *eval(LEXEME *tree, LEXEME *env);
static LEXEME *evalProgram(LEXEME *tree, LEXEME *env);
static LEXEME *evalMain(LEXEME *tree, LEXEME *env);
static LEXEME *evalClassDef(LEXEME *tree, LEXEME *env);
static LEXEME *evalClassStatements(LEXEME *tree, LEXEME *env);
static LEXEME *evalClassStatement(LEXEME *tree, LEXEME *env);
static LEXEME *evalVarDecl(LEXEME *tree, LEXEME *env);
static LEXEME *evalFunctionStatement(LEXEME *tree, LEXEME *env);
static LEXEME *evalParen(LEXEME *tree, LEXEME *env);
static LEXEME *evalUnaryID(LEXEME *tree, LEXEME *env);
static LEXEME *evalFunctionCall(LEXEME *tree, LEXEME *env);
static LEXEME *evalConstructor(LEXEME *tree);
static LEXEME *evalBuiltIn(LEXEME *tree, LEXEME *args);
static LEXEME *evalClosure(LEXEME *closure, LEXEME *args);
static LEXEME *evalArgs(LEXEME *args, LEXEME *env);
static LEXEME *evalStatements(LEXEME *tree, LEXEME *env);
static LEXEME *evalNewObj(LEXEME *tree, LEXEME *env);
static LEXEME *evalIf(LEXEME *tree, LEXEME *env);
static LEXEME *evalElse(LEXEME *tree, LEXEME *env);
static LEXEME *evalWhile(LEXEME *tree, LEXEME *env);
static LEXEME *evalDoWhile(LEXEME *tree, LEXEME *env);
static LEXEME *evalReturn(LEXEME *tree, LEXEME *env);
static LEXEME *evalLambda(LEXEME *tree, LEXEME *env);
// evalOperator
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
// evalBuiltIn
static LEXEME *evalNewArray(LEXEME *args);
static LEXEME *evalGetArray(LEXEME *args);
static LEXEME *evalSetArray(LEXEME *args);
static LEXEME *evalPrint(LEXEME *args);
static LEXEME *evalPrintLn(LEXEME *args);

static void failExpr(char *expected, char *exprType, LEXEME *badLex);

LEXEME *mainArgs = NULL;

int main(int argc, char **argv)
{
    char *filename = parseFileArg(argc, argv);

    LEXEME *globalEnv = newEnvironment();
    addBuiltIn(globalEnv);
    
    LEXEME *rootLex = parse(filename);

    LEXEME *returnVal = eval(rootLex, globalEnv);

    //printf("Main type: %s\n", getTypeLEXEME(returnVal));
    //printf("Returned lexeme(%s): ", getTypeLEXEME(returnVal));
    //displayLEXEME(stdout, returnVal);
    //printf("\n");
    
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
        arg = newLEXEMEstring(STRING, argv[i], -1);
        mainArgs = cons(VAR_LIST, arg, mainArgs);
    }
    
    return argv[1];
}

static void addBuiltIn(LEXEME *env)
{
    // Functions
    insertEnvironment(env, newLEXEMEstring(ID, "newArray", -1), newLEXEMEfunction(BUILT_IN, evalNewArray));
    insertEnvironment(env, newLEXEMEstring(ID, "getArray", -1), newLEXEMEfunction(BUILT_IN, evalGetArray));
    insertEnvironment(env, newLEXEMEstring(ID, "setArray", -1), newLEXEMEfunction(BUILT_IN, evalSetArray));
    insertEnvironment(env, newLEXEMEstring(ID, "print", -1), newLEXEMEfunction(BUILT_IN, evalPrint));
    insertEnvironment(env, newLEXEMEstring(ID, "println", -1), newLEXEMEfunction(BUILT_IN, evalPrintLn));
    // Variables
    insertEnvironment(env, newLEXEMEstring(ID, "false", -1), newLEXEMEint(0, -1));
    insertEnvironment(env, newLEXEMEstring(ID, "true", -1), newLEXEMEint(1, -1));
}

static LEXEME *eval(LEXEME *tree, LEXEME *env)
{
    char *type = getTypeLEXEME(tree);
    if (isPrimative(tree))  return tree;    // Int, real, or str
    //if (type == ID) return getValueEnv(env, tree);
    if (type == PROG)   return evalProgram(tree, env);
    if (type == MAIN_FUNCTION)  return evalMain(tree, env);
    if (type == CLASS_DEF)  return evalClassDef(tree, env);
    if (type == CLASS_STATEMENTS)   return evalClassStatements(tree, env);
    if (type == CLASS_STATEMENT)    return evalClassStatement(tree, env);
    if (type == VAR_DECL)   return evalVarDecl(tree, env);
    if (type == FUNCTION_STATEMENT) return evalFunctionStatement(tree, env);
    if (type == UNARY_PAREN)    return evalParen(tree, env);
    if (type == UNARY_ID)   return evalUnaryID(tree, env);
    if (type == FUNCTION_CALL)  return evalFunctionCall(tree, env);
    if (type == EXPR_LIST)  return evalArgs(tree, env);
    if (type == CLOSURE)    return evalClosure(tree, env);
    if (type == STATEMENTS) return evalStatements(tree, env);
    if (type == NEW_OBJECT) return evalNewObj(tree, env);
    if (type == IF_STATEMENT || type == ELSE_IF_STATEMENT)  return evalIf(tree, env);
    if (type == ELSE_STATEMENT) return evalElse(tree, env);
    if (type == WHILE_STATEMENT)    return evalWhile(tree, env);
    if (type == DO_WHILE_STATEMENT) return evalDoWhile(tree, env);
    if (type == RETURN_STATEMENT)   return evalReturn(tree, env);
    if (type == LAMBDA_STATEMENT)   return evalLambda(tree, env);
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
    fprintf(stderr, "Unhandled LEXEME in eval(). Type %s\n", getTypeLEXEME(tree));
    exit(-500);
}

static LEXEME *evalProgram(LEXEME *tree, LEXEME *env)
{
    LEXEME *returnVal = eval(car(tree), env);   // ClassDef or Main
    if(cdr(tree))   return eval(cdr(tree), env);    // Program
    return returnVal;
}

static LEXEME *evalMain(LEXEME *tree, LEXEME *env)
{
    //displayEnvironment(stdout, env);
    LEXEME *result = evalStatements(car(tree), env);
    if (getTypeLEXEME(result) == RETURNED)  return car(result);
    fprintf(stderr, "Main function did not return a value. Instead returned type %s\n", getTypeLEXEME(result));
    exit(-9001);
}

static LEXEME *evalClassDef(LEXEME *tree, LEXEME *env)
{
    if (cdr(car(tree)))
    {
        // If we need to extend from a previous class - TODO
    }
    LEXEME *closure = cons(OCLOSURE, env, tree);
    insertEnvironment(env, car(car(tree)), closure);
    
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
    LEXEME *varDef, *value;
    tree = car(tree);   // VarList
    while (tree)
    {
        varDef = car(tree);
        if (cdr(varDef))
            value = eval(cdr(varDef), env); // Expr
        else
            value = newLEXEME(NULL_VALUE, -1);
        insertEnvironment(env, car(varDef), value);  // ID - var name
        tree = cdr(tree);
    }
    return env;
}

static LEXEME *evalFunctionStatement(LEXEME *tree, LEXEME *env)
{
    LEXEME *closure = cons(CLOSURE, env, tree);
    insertEnvironment(env, car(cdr(tree)), closure);
    
    return closure;
}

static LEXEME *evalParen(LEXEME *tree, LEXEME *env)
{
    return eval(car(tree), env);
}

static LEXEME *evalUnaryID(LEXEME *tree, LEXEME *env)
{
    char *idType = getTypeLEXEME(car(tree));
    if (idType == ID)
    {
        return getValueEnv(env, car(tree));
    }
    if (idType == FUNCTION_CALL)
    {
        LEXEME *returned = eval(car(tree), env);    // FunctionCall
        return returned;
    }
    if (idType == ARRAY_LOOKUP)
    {
        LEXEME *array = getValueEnv(env, car(car(tree)));
        LEXEME *index = cdr(car(tree));
        return getArrayValueLEXEME(array, getIntLEXEME(index));
    }
    fprintf(stderr, "Invalid IDExpr given in UnaryID. Type given: %s\n", getTypeLEXEME(car(tree)));
    exit(-204);
}

static LEXEME *evalFunctionCall(LEXEME *tree, LEXEME *env)
{
    LEXEME *closure = getValueEnv(env, car(tree));
    char *closureType = getTypeLEXEME(closure);
    LEXEME *args = evalArgs(cdr(tree), env);
    if (closureType == BUILT_IN)    return evalBuiltIn(closure, args);
    if (closureType == OCLOSURE)    return evalConstructor(closure);
    if (closureType == CLOSURE)     return evalClosure(closure, args);
    fprintf(stderr, "Invalid closure type given in function call\n");
    exit(-210);
}

static LEXEME *evalConstructor(LEXEME *closure)
{
    LEXEME *staticEnv = car(closure);
    LEXEME *extendedEnv = newScopeEnv(staticEnv, NULL, NULL);
    LEXEME *body = cdr(cdr(closure));
    insertEnvironment(extendedEnv, newLEXEMEstring(ID, "this", -1), extendedEnv);
    evalStatements(body, extendedEnv);
    //printf("EXTENDED ENVIRONMENT:\n");
    //displayEnvironment(stdout, extendedEnv);
    return extendedEnv;
}

static LEXEME *evalBuiltIn(LEXEME *tree, LEXEME *args)
{
    return evalFunctionLEXEME(tree, args);
}

static LEXEME *evalClosure(LEXEME *closure, LEXEME *args)
{
    LEXEME *staticEnv = car(closure);
    LEXEME *params = cdr(cdr(cdr(closure)));
    LEXEME *paramIter = params;
    while (paramIter)
    {
        // TODO add support for opt args
        setCar(car(car(paramIter)));    // Update the value in the list to be ID, not VAR_DEF
        paramIter = cdr(paramIter);
    }
    LEXEME *localEnv = newScopeEnv(staticEnv, params, args);
    LEXEME *body = car(cdr(closure));
    LEXEME *result = evalStatements(body, localEnv);
    if (getTypeLEXEME(result) == RETURNED)  return car(result);
    return result;
}

static LEXEME *evalArgs(LEXEME *args, LEXEME *env)
{
    if (args == NULL)   return NULL;
    return cons(EVAL_EXPR, eval(car(args), env), evalArgs(cdr(args), env));
}

static LEXEME *evalStatements(LEXEME *tree, LEXEME *env)
{
    LEXEME *result;
    while (tree)
    {
        result = eval(car(tree), env);  // Statement - if, else, etc - TODO
        if (result == NULL) result = newLEXEME(NULL_VALUE, -1);
        if (getTypeLEXEME(result) == RETURNED)
            break;
        tree = cdr(tree);
    }
    return result;
}

static LEXEME *evalNewObj(LEXEME *tree, LEXEME *env)
{
    LEXEME *classClosure = getValueEnv(env, car(tree)); // Find class with ID of car(tree)
    
    return evalConstructor(classClosure);
}

static LEXEME *evalIf(LEXEME *tree, LEXEME *env)
{
    LEXEME *exprResult = eval(car(cdr(tree)), env);
    assert(getTypeLEXEME(exprResult) == INTEGER);   // Should be a boolean(int)
    if (getIntLEXEME(exprResult))   return eval(cdr(cdr(tree)), env);   // Statements
    else if (car(tree)) // If there is an else statement to be evaluated
    {
        return eval(car(tree), env);    // else or else if
    }
    
    return exprResult;  // Return 0
}

static LEXEME *evalElse(LEXEME *tree, LEXEME *env)
{
    return eval(car(tree), env);    // statements
}

static LEXEME *evalWhile(LEXEME *tree, LEXEME *env)
{
    LEXEME *exprResult = eval(car(tree), env), *bodyResult;
    assert(getTypeLEXEME(exprResult) == INTEGER);   // Should be a boolean(int)
    while (getIntLEXEME(exprResult))
    {
        bodyResult = eval(cdr(tree), env);
        if (getTypeLEXEME(bodyResult) == RETURNED)  return bodyResult;
        exprResult = eval(car(tree), env);
    }
    return bodyResult;
}

static LEXEME *evalDoWhile(LEXEME *tree, LEXEME *env)
{
    LEXEME *exprResult, *bodyResult = eval(car(tree), env);
    if (getTypeLEXEME(bodyResult) == RETURNED)  return bodyResult;
    exprResult = eval(cdr(tree), env);
    assert(getTypeLEXEME(exprResult) == INTEGER);   // Should be a boolean(int)
    while (getIntLEXEME(exprResult))
    {
        bodyResult = eval(car(tree), env);
        if (getTypeLEXEME(bodyResult) == RETURNED)  return bodyResult;
        exprResult = eval(cdr(tree), env);
    }
    return bodyResult;
}

static LEXEME *evalReturn(LEXEME *tree, LEXEME *env)
{
    LEXEME *returned = newLEXEME(RETURNED, -1);
    setCar(returned, eval(car(tree), env));
    return returned;
}

static LEXEME *evalLambda(LEXEME *tree, LEXEME *env)
{
    return cons(CLOSURE, env, tree);
}

static LEXEME *evalEquals(LEXEME *tree, LEXEME *env)
{
    LEXEME *left = car(tree);
    //printf("left lexeme(%s): ", getTypeLEXEME(left));
    //displayLEXEME(stdout, left);
    //printf("\tright lexeme: ");
    //displayLEXEME(stdout, cdr(tree));
    //printf("\n");
    if (getTypeLEXEME(left) == UNARY_ID)
    {
        LEXEME *right = eval(cdr(tree), env);
        if (getTypeLEXEME(car(left)) == ARRAY_LOOKUP)
        {
            LEXEME *array = getValueEnv(env, car(car(left)));
            setArrayValueLEXEME(array, getIntLEXEME(cdr(car(left))), right);
        }
        else if (getTypeLEXEME(car(left)) == ID)    setValueEnv(env, car(left), right);
        else    failExpr("ID or ArrayLookup", "equals", car(left));
        return right;
    }
    failExpr("UnaryID", "equals", left);
    exit(-290);
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
    return eval(cdr(tree), object);
}

static void failExpr(char *expected, char *exprType, LEXEME *badLex)
{
    fprintf(stderr, "Expected type %s in %s expression. Got %s\n", expected, exprType, getTypeLEXEME(badLex));
    exit(-101);
}

static LEXEME *evalNewArray(LEXEME *args)
{
    assert(cdr(args) == NULL);  // 1 argument
    LEXEME *size = car(args);
    assert(getTypeLEXEME(size) == INTEGER);  // Argument is an integer
    return newLEXEMEarray(getIntLEXEME(size));
}

static LEXEME *evalGetArray(LEXEME *args)
{
    assert(cdr(args) && cdr(cdr(args)) == NULL);    // 2 arguments
    LEXEME *array = car(args);  // 1st arg is the array
    LEXEME *index = car(cdr(args)); // 2nd arg is the array index
    assert(getTypeLEXEME(array) == ARRAY && getTypeLEXEME(index) == INTEGER);
    return getArrayValueLEXEME(array, getIntLEXEME(index));
}

/*
 * Sets a new lexeme value for the given array at the given index.
 * Returns the old value at that location.
 */
static LEXEME *evalSetArray(LEXEME *args)
{
    assert(cdr(args) && cdr(cdr(args)) && cdr(cdr(cdr(args))) == NULL); // 3 arguments
    LEXEME *array = car(args);  // 1st arg is the array
    LEXEME *index = car(cdr(args)); // 2nd arg is the array index
    LEXEME *newVal = car(cdr(cdr(args)));   // 3rd arg is the new value
    assert(getTypeLEXEME(array) == ARRAY && getTypeLEXEME(index) == INTEGER);
    return setArrayValueLEXEME(array, getIntLEXEME(index), newVal);
}

static LEXEME *evalPrint(LEXEME *args)
{
    assert(car(args));  // Make sure there is an argument
    LEXEME *output = car(args);  // 1st arg is the output string
    char *outType = getTypeLEXEME(output);
    if (outType == STRING)        printf("%s", getStrLEXEME(output));
    else if (outType == INTEGER)  printf("%d", getIntLEXEME(output));
    else if (outType == REAL)     printf("%f", getRealLEXEME(output));
    else
    {
        fprintf(stderr, "Invalid type given to print function. Given: %s\n", getTypeLEXEME(output));
        exit(-103);
    }
    
    if (cdr(args) != NULL)  return evalPrint(cdr(args));    // Continue printing arguments
    
    return output;
}

static LEXEME *evalPrintLn(LEXEME *args)
{
    assert(car(args));  // Make sure there is an argument
    LEXEME *output = car(args);  // 1st arg is the output string
    char *outType = getTypeLEXEME(output);
    if (outType == STRING)        printf("%s", getStrLEXEME(output));
    else if (outType == INTEGER)  printf("%d", getIntLEXEME(output));
    else if (outType == REAL)     printf("%f", getRealLEXEME(output));
    else
    {
        fprintf(stderr, "Invalid type given to println function. Given: %s\n", getTypeLEXEME(output));
        exit(-103);
    }
    
    if (cdr(args) == NULL)  printf("\n");   // Nothing more to print, print the newline
    else    return evalPrintLn(cdr(args));  // Continue printing arguments
    
    return output;
}

