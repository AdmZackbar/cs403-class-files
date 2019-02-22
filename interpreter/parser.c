#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "lexeme.h"
#include "types.h"

// Normal helper functions
static void failParse(char *component);
// Grammar helper functions
static LEXEME *program();
static int programPending();
static LEXEME *mainFunc();
static LEXEME *classDef();
static int classDefPending();
static LEXEME *classHeader();
//static int classHeaderPending();
static LEXEME *optClassStatements();
//static int optClassStatementsPending();
static LEXEME *classStatements();
static int classStatementsPending();
static LEXEME *classStatement();
static int classStatementPending();
static LEXEME *accessMod();
static int accessModPending();
static LEXEME *optVarList();
//static int optVarListPending();
static LEXEME *varList();
static int varListPending();
static LEXEME *varDef();
static int varDefPending();
static LEXEME *expr();
static int exprPending();
static LEXEME *expr2();
static LEXEME *expr3();
static LEXEME *expr4();
static LEXEME *expr5();
static LEXEME *expr6();
static LEXEME *expr7();
static LEXEME *expr8();
static LEXEME *unary();
static int unaryPending();
static LEXEME *idExpr();
static int idExprPending();
static LEXEME *postVar();
static int postVarPending();
static LEXEME *optExprList();
//static int optExprListPending();
static LEXEME *exprList();
static int exprListPending();
static LEXEME *uop();
static int uopPending();
static LEXEME *block();
static int blockPending();
static LEXEME *optStatements();
//static int optStatementsPending();
static LEXEME *statements();
static int statementsPending();
static LEXEME *statement();
static int statementPending();
static LEXEME *optElse();
//static int optElsePending();
static LEXEME *elseStatement();
static int elseStatementPending();

LEXER *lexer;
LEXEME *current;

LEXEME *advance()
{
    LEXEME *prev = current;
    current = lex(lexer);
    return prev;
}

int check(char *type)
{
    return getTypeLEXEME(current) == type;
}

LEXEME *match(char *type)
{
    if(check(type))
        return advance();
        
    printf("SYNTAX ERROR: expected %s got %s - line %d\n", type, getTypeLEXEME(current), getLineNumLEXEME(current));
    exit(-2);
}

LEXEME *parse(char *filename)
{
    // Initialize globals
    lexer = newLEXER(filename);
    current = lex(lexer);
    
    // Highest level of program
    LEXEME *programLex = program();
    
    // Check if everything has been properly parsed
    match(END_OF_FILE);
    
    return programLex;
}

static void failParse(char *component)
{
    printf("Incorrect structure in grammar module %s\n", component);
    exit(-404);
}

static LEXEME *program()
{
    LEXEME *classLex = classDef();
    if(programPending())
        return cons(PROG, classLex, program());
    return cons(PROG, classLex, mainFunc());
}
static int programPending()
{
    return classDefPending();
}

static LEXEME *mainFunc()
{
    match(MAIN);
    match(OPAREN);
    match(ARGS);
    match(CPAREN);
    return cons(MAIN_FUNCTION, block(), NULL);
}

static LEXEME *classDef()
{
    LEXEME *headerLex, *statementsLex;
    match(CLASS);
    headerLex = classHeader();
    match(OBRACE);
    statementsLex = optClassStatements();
    match(CBRACE);
    
    return cons(CLASS_DEF, headerLex, statementsLex);
}
static int classDefPending()
{
    return check(CLASS);
}

static LEXEME *classHeader()
{
    LEXEME *idLex, *extendsIDLex = NULL;
    idLex = match(ID);
    if (check(EXTENDS))
    {
        advance();
        extendsIDLex = match(ID);
    }
    
    return cons(CLASS_HEADER, idLex, extendsIDLex);
}
//static int classHeaderPending()
//{
//    return check(ID);
//}

static LEXEME *optClassStatements()
{
    if (classStatementsPending())
        return classStatements();
    
    return NULL;
}
//static int optClassStatementsPending()
//{
//    return 1;
//}

static LEXEME *classStatements()
{
    LEXEME *statementLex, *statementsLex = NULL;
    statementLex = classStatement();
    if (classStatementsPending())
        statementsLex = classStatements();
    
    return cons(CLASS_STATEMENTS, statementLex, statementsLex);
}
static int classStatementsPending()
{
    return classStatementPending();
}

static LEXEME *classStatement()
{
    LEXEME *accessModLex, *statementLex;
    accessModLex = accessMod();
    if (check(VAR))
    {
        advance();
        statementLex = cons(VAR_DECL, varList(), NULL);
        match(SEMICOLON);
    }
    else if (check(FUNCTION))
    {
        LEXEME *functionInfo, *functionName, *functionParams, *blockLex;
        advance();
        functionName = match(ID);
        match(OPAREN);
        functionParams = optVarList();
        match(CPAREN);
        blockLex = block();
        
        functionInfo = cons(FUNCTION_INFO, functionName, functionParams);
        statementLex = cons(FUNCTION_STATEMENT, blockLex, functionInfo);
    }
    else
        failParse("class statement");
    
    return cons(CLASS_STATEMENT, accessModLex, statementLex);
}
static int classStatementPending()
{
    return accessModPending() || check(VAR) || check(FUNCTION);
}

static LEXEME *accessMod()
{
    if (check(PUBLIC))
        return advance();
    if (check(PRIVATE))
        return advance();
    if (check(PROTECTED))
        return advance();
    return NULL;
}
static int accessModPending()
{
    return check(PUBLIC) || check(PRIVATE) || check(PROTECTED);
}

static LEXEME *optVarList()
{
    if(varListPending())
        return varList();
    
    return NULL;
}
//static int optVarListPending()
//{
//    return varListPending();
//}

static LEXEME *varList()
{
    LEXEME *varDefLex, *varListLex = NULL;
    varDefLex = varDef();
    if (check(COMMA))
    {
        advance();
        varListLex = varList();
    }
    
    return cons(VAR_LIST, varDefLex, varListLex);
}
static int varListPending()
{
    return varDefPending();
}

static LEXEME *varDef()
{
    LEXEME *idLex, *exprLex = NULL;
    idLex = match(ID);
    if (check(EQUALS))
    {
        advance();
        exprLex = expr();
    }
    
    return cons(VAR_DEF, idLex, exprLex);
}
static int varDefPending()
{
    return check(ID);
}

static LEXEME *expr()
{
    LEXEME *unaryLex, *exprLex, *opLex;
    unaryLex = expr2();
    if (check(EQUALS))
    {
        opLex = advance();
        exprLex = expr();
        setCar(opLex, unaryLex);
        setCdr(opLex, exprLex);
        return opLex;
    }
    
    return unaryLex;
}
static int exprPending()
{
    return unaryPending();
}

static LEXEME *expr2()
{
    LEXEME *unaryLex, *exprLex, *opLex;
    unaryLex = expr3();
    while (check(LOGICAL_AND) || check(LOGICAL_OR))
    {
        opLex = advance();
        exprLex = expr3();
        setCar(opLex, unaryLex);
        setCdr(opLex, exprLex);
        unaryLex = opLex;
    }
    
    return unaryLex;
}
static LEXEME *expr3()
{
    LEXEME *unaryLex, *exprLex, *opLex;
    unaryLex = expr4();
    while (check(BINARY_AND) || check(BINARY_OR))
    {
        opLex = advance();
        exprLex = expr4();
        setCar(opLex, unaryLex);
        setCdr(opLex, exprLex);
        unaryLex = opLex;
    }
    
    return unaryLex;
}
static LEXEME *expr4()
{
    LEXEME *unaryLex, *exprLex, *opLex;
    unaryLex = expr5();
    while (check(EQUALSEQUALS) || check(NOTEQUALS) || check(LESS_THAN)
        || check(GREATER_THAN) || check(LESS_THAN_EQUAL) || check(GREATER_THAN_EQUAL))
    {
        opLex = advance();
        exprLex = expr5();
        setCar(opLex, unaryLex);
        setCdr(opLex, exprLex);
        unaryLex = opLex;
    }
    
    return unaryLex;
}
static LEXEME *expr5()
{
    LEXEME *unaryLex, *exprLex, *opLex;
    unaryLex = expr6();
    while (check(PLUS) || check(MINUS))
    {
        opLex = advance();
        exprLex = expr6();
        setCar(opLex, unaryLex);
        setCdr(opLex, exprLex);
        unaryLex = opLex;
    }
    
    return unaryLex;
}
static LEXEME *expr6()
{
    LEXEME *unaryLex, *exprLex, *opLex;
    unaryLex = expr7();
    while (check(TIMES) || check(DIVIDE) || check(MODULUS))
    {
        opLex = advance();
        exprLex = expr7();
        setCar(opLex, unaryLex);
        setCdr(opLex, exprLex);
        unaryLex = opLex;
    }
    
    return unaryLex;
}
static LEXEME *expr7()
{
    LEXEME *unaryLex, *exprLex, *opLex;
    unaryLex = expr8();
    while (check(EXPONENT))
    {
        opLex = advance();
        exprLex = expr8();
        setCar(opLex, unaryLex);
        setCdr(opLex, exprLex);
        unaryLex = opLex;
    }
    
    return unaryLex;
}
static LEXEME *expr8()
{
    LEXEME *unaryLex, *exprLex, *opLex;
    unaryLex = unary();
    while (check(DOT))
    {
        opLex = advance();
        exprLex = unary();
        setCar(opLex, unaryLex);
        setCdr(opLex, exprLex);
        unaryLex = opLex;
        if (getTypeLEXEME(cdr(unaryLex)) == FUNCTION_CALL)
            return cons(FUNCTION_CALL, cons(DOT, car(unaryLex), car(cdr(unaryLex))), cdr(cdr(unaryLex)));
    }
    
    return unaryLex;
}

static LEXEME *unary()
{
    if (idExprPending())
    {
        LEXEME *idExprLex, *postVarLex = NULL;
        idExprLex = idExpr();
        if (postVarPending())
            postVarLex = postVar();
        
        return cons(UNARY_ID, idExprLex, postVarLex);
    }
    else if (check(INTEGER))
        return advance();
    else if (check(REAL))
        return advance();
    else if (check(STRING))
        return advance();
    else if (uopPending())
    {
        LEXEME *uopLex, *unaryLex;
        uopLex = uop();
        unaryLex = unary();
        return cons(UNARY_OP, uopLex, unaryLex);
    }
    else if (check(OPAREN))
    {
        LEXEME *exprLex;
        advance();
        exprLex = expr();
        match(CPAREN);
        return cons(UNARY_PAREN, exprLex, NULL);
    }
    else if (check(NULL_WORD))
        return advance();
    else if (check(NEW))
    {
        LEXEME *idLex, *exprListLex;
        advance();
        idLex = match(ID);
        match(OPAREN);
        exprListLex = optExprList();
        match(CPAREN);
        return cons(NEW_OBJECT, idLex, exprListLex);
    }
    else if (check(LAMBDA))
    {
        LEXEME *params, *blockLex;
        advance();
        match(OPAREN);
        params = optVarList();
        match(CPAREN);
        blockLex = block();
        
        return cons(LAMBDA_STATEMENT, blockLex, cons(FUNCTION_INFO, NULL, params));
    }
    failParse("unary");
    return NULL;    // Unreachable - for compiler
}
static int unaryPending()
{
    return idExprPending() || check(INTEGER) || check(REAL) || check(STRING)
            || uopPending() || check(OPAREN) || check(NULL_WORD)
            || check(NEW) || check(LAMBDA);
}

static LEXEME *idExpr()
{
    LEXEME *idLex, *postIDLex = NULL;
    idLex = match(ID);
    if (check(OPAREN))
    {
        advance();
        postIDLex = optExprList();
        match(CPAREN);
        return cons(FUNCTION_CALL, idLex, postIDLex);
    }
    if (check(OBRACKET))
    {
        advance();
        postIDLex = expr();
        match(CBRACKET);
        return cons(ARRAY_LOOKUP, idLex, postIDLex);
    }
    
    return idLex;
}
static int idExprPending()
{
    return check(ID);
}

static LEXEME *postVar()
{
    if (check(PLUSPLUS))
        return advance();
    else if (check(MINUSMINUS))
        return advance();
    
    return NULL;
}
static int postVarPending()
{
    return check(PLUSPLUS) || check(MINUSMINUS);
}

static LEXEME *optExprList()
{
    if (exprListPending())
        return exprList();
    
    return NULL;
}
//static int optExprListPending()
//{
//    return exprListPending();
//}

static LEXEME *exprList()
{
    LEXEME *exprLex, *exprListLex = NULL;
    exprLex = expr();
    if (check(COMMA))
    {
        advance();
        exprListLex = exprList();
    }
    
    return cons(EXPR_LIST, exprLex, exprListLex);
}
static int exprListPending()
{
    return exprPending();
}

static LEXEME *uop()
{
    if (check(MINUS))
    {
        // Change minus to the unary version
        LEXEME *uop = advance();
        return newLEXEME(UMINUS, getLineNumLEXEME(uop));
    }
    else if (check(PLUSPLUS))
        return advance();
    else if (check(MINUSMINUS))
        return advance();
    else if (check(NOT))
        return advance();
    failParse("unary operator");
    return NULL;    // Unreachable - for compiler
}
static int uopPending()
{
    return check(MINUS) || check(PLUSPLUS) || check(MINUSMINUS) || check(NOT);
}

static LEXEME *block()
{
    LEXEME *statementsLex;
    match(OBRACE);
    statementsLex = optStatements();
    match(CBRACE);
    
    return statementsLex;
}
static int blockPending()
{
    return check(OBRACE);
}

static LEXEME *optStatements()
{
    if (statementsPending())
        return statements();
    
    return NULL;
}
//static int optStatementsPending()
//{
//    return statementsPending();
//}

static LEXEME *statements()
{
    LEXEME *statementLex, *statementsLex = NULL;
    statementLex = statement();
    if (statementsPending())
        statementsLex = statements();
    
    return cons(STATEMENTS, statementLex, statementsLex);
}
static int statementsPending()
{
    return statementPending();
}

static LEXEME *statement()
{
    LEXEME *statementLex;
    if (exprPending())
    {
        statementLex = expr();
        match(SEMICOLON);
    }
    else if (check(VAR))
    {
        advance();
        statementLex = cons(VAR_DECL, varList(), NULL);
        match(SEMICOLON);
    }
    else if (check(IF))
    {
        LEXEME *ifStatement, *exprLex, *blockLex, *optElseLex;
        advance();
        match(OPAREN);
        exprLex = expr();
        match(CPAREN);
        blockLex = block();
        optElseLex = optElse();
        
        ifStatement = cons(IF_BODY, exprLex, blockLex);
        statementLex = cons(IF_STATEMENT, optElseLex, ifStatement);
    }
    else if (check(WHILE))
    {
        LEXEME *exprLex, *blockLex;
        advance();
        match(OPAREN);
        exprLex = expr();
        match(CPAREN);
        blockLex = block();
        
        statementLex = cons(WHILE_STATEMENT, exprLex, blockLex);
    }
    else if (check(DO))
    {
        LEXEME *blockLex, *exprLex;
        advance();
        blockLex = block();
        match(WHILE);
        match(OPAREN);
        exprLex = expr();
        match(CPAREN);
        match(SEMICOLON);
        
        statementLex = cons(DO_WHILE_STATEMENT, blockLex, exprLex);
    }
    else if (check(RETURN))
    {
        LEXEME *exprLex = NULL;
        advance();
        if(exprPending())
            exprLex = expr();
        match(SEMICOLON);
        
        statementLex = cons(RETURN_STATEMENT, exprLex, NULL);
    }
    else if (check(FUNCTION))
    {
        advance();
        LEXEME *functionName = match(ID);
        match(OPAREN);
        LEXEME *functionParams = optVarList();
        match(CPAREN);
        LEXEME *blockLex = block();
        functionInfo = cons(FUNCTION_INFO, functionName, functionParams);
        statementLex = cons(FUNCTION_STATEMENT, blockLex, functionInfo);
    }
    else
        failParse("statement");
    
    return statementLex;
}
static int statementPending()
{
    return exprPending() || check(VAR) || check(IF) || check(WHILE) || check(DO)
        || check(RETURN);
}

static LEXEME *optElse()
{
    if (elseStatementPending())
        return elseStatement();
    
    return NULL;
}
//static int optElsePending()
//{
//    return elseStatementPending();
//}

static LEXEME *elseStatement()
{   
    match(ELSE);
    if (blockPending())
    {
        LEXEME *blockLex = block();
        return cons(ELSE_STATEMENT, blockLex, NULL);
    }
    else if (check(IF))
    {
        LEXEME *ifStatement, *exprLex, *blockLex, *optElseLex;
        advance();
        match(OPAREN);
        exprLex = expr();
        match(CPAREN);
        blockLex = block();
        optElseLex = optElse();
        
        ifStatement = cons(IF_BODY, exprLex, blockLex);
        return cons(ELSE_IF_STATEMENT, optElseLex, ifStatement);
    }
    else
        failParse("else statement");
    
    return NULL;    // unreachable - for compiler
}
static int elseStatementPending()
{
    return check(ELSE);
}
