#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "lexeme.h"
#include "types.h"

// Normal helper functions
static char *parseFileArg(int argc, char **argv);
static LEXEME *advance();
static int check(char *type);
static LEXEME *match(char *type);
static void recognize(char *filename);
static void failParse(char *component);
// Grammar helper functions
static void program();
static int programPending();
static void classDef();
static int classDefPending();
static void classHeader();
static int classHeaderPending();
static void optClassStatements();
static int optClassStatementsPending();
static void classStatements();
static int classStatementsPending();
static void classStatement();
static int classStatementPending();
static void accessMod();
static int accessModPending();
static void varList();
static int varListPending();
static void varDef();
static int varDefPending();
static void expr();
static int exprPending();
static void unary();
static int unaryPending();
static void idExpr();
static int idExprPending();
static void postVar();
static int postVarPending();
static void optExprList();
static int optExprListPending();
static void exprList();
static int exprListPending();
static void uop();
static int uopPending();
static void op();
static int opPending();
static void optParamList();
static int optParamListPending();
static void paramList();
static int paramListPending();
static void param();
static int paramPending();
static void block();
static int blockPending();
static void optStatements();
static int optStatementsPending();
static void statements();
static int statementsPending();
static void statement();
static int statementPending();
static void optElse();
static int optElsePending();
static void elseStatement();
static int elseStatementPending();

LEXER *lexer;
LEXEME *current;

int main(int argc, char **argv)
{
    char *filename = parseFileArg(argc, argv);
    
    recognize(filename);
    
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

static LEXEME *advance()
{
    LEXEME *prev = current;
    current = lex(lexer);
    return prev;
}
static int check(char *type)
{
    return getTypeLEXEME(current) == type;
}
static LEXEME *match(char *type)
{
    if(check(type))
        return advance();
        
    printf("SYNTAX ERROR: expected %s got %s - line %d\n", type, getTypeLEXEME(current), getLineNumLEXEME(current));
    exit(-2);
}
static void recognize(char *filename)
{
    // Initialize globals
    lexer = newLEXER(filename);
    current = lex(lexer);
    
    // Highest level of program
    program();
    
    // Check if everything has been properly parsed
    match(END_OF_FILE);
}
static void failParse(char *component)
{
    printf("Incorrect structure in grammar module %s\n", component);
    exit(-404);
}

static void program()
{
    classDef();
    if(programPending())
        program();
}
static int programPending()
{
    return classDefPending();
}

static void classDef()
{
    match(CLASS);
    classHeader();
    match(OBRACE);
    optClassStatements();
    match(CBRACE);
}
static int classDefPending()
{
    return check(CLASS);
}

static void classHeader()
{
    match(ID);
    if (check(EXTENDS))
    {
        advance();
        match(ID);
    }
}
static int classHeaderPending()
{
    return check(ID);
}

static void optClassStatements()
{
    if (classStatementsPending())
        classStatements();
}
static int optClassStatementsPending()
{
    return 1;
}

static void classStatements()
{
    classStatement();
    if (classStatementsPending())
        classStatements();
}
static int classStatementsPending()
{
    return classStatementPending();
}

static void classStatement()
{
    accessMod();
    if (check(VAR))
    {
        advance();
        varList();
        match(SEMICOLON);
    }
    else if (check(FUNCTION))
    {
        advance();
        match(ID);
        match(OPAREN);
        optParamList();
        match(CPAREN);
        block();
    }
    else
        failParse("class statement");
}
static int classStatementPending()
{
    return accessModPending() || check(VAR) || check(FUNCTION);
}

static void accessMod()
{
    if (check(PUBLIC))
        advance();
    if (check(PRIVATE))
        advance();
    if (check(PROTECTED))
        advance();
}
static int accessModPending()
{
    return check(PUBLIC) || check(PRIVATE) || check(PROTECTED);
}

static void varList()
{
    varDef();
    if (check(COMMA))
    {
        advance();
        varList();
    }
}
static int varListPending()
{
    return varDefPending();
}

static void varDef()
{
    match(ID);
    if (check(EQUALS))
    {
        advance();
        expr();
    }
}
static int varDefPending()
{
    return check(ID);
}

static void expr()
{
    unary();
    if (opPending())
    {
        op();
        expr();
    }
}
static int exprPending()
{
    return unaryPending();
}

static void unary()
{
    if (idExprPending())
        idExpr();
    else if (check(INTEGER))
        advance();
    else if (check(REAL))
        advance();
    else if (check(STRING))
        advance();
    else if (uopPending())
    {
        uop();
        unary();
    }
    else if (check(OPAREN))
    {
        advance();
        expr();
        match(CPAREN);
    }
    else if (check(NULL_WORD))
        advance();
    else if (check(THIS))
        advance();
    else if (check(NEW))
    {
        advance();
        match(ID);
        match(OPAREN);
        optExprList();
        match(CPAREN);
    }
    else
        failParse("unary");
}
static int unaryPending()
{
    return idExprPending() || check(INTEGER) || check(REAL) || check(STRING)
            || uopPending() || check(OPAREN) || check(NULL_WORD) || check(THIS)
            || check(NEW);
}

static void idExpr()
{
    match(ID);
    if (postVarPending())
        postVar();
    else if (check(OPAREN))
    {
        advance();
        optExprList();
        match(CPAREN);
    }
    else if (check(OBRACKET))
    {
        advance();
        expr();
        match(CBRACKET);
    }
}
static int idExprPending()
{
    return check(ID);
}

static void postVar()
{
    if (check(PLUSPLUS))
        advance();
    else if (check(MINUSMINUS))
        advance();
    // TODO ERROR OUT
}
static int postVarPending()
{
    return check(PLUSPLUS) || check(MINUSMINUS);
}

static void optExprList()
{
    if (exprListPending())
        exprList();
}
static int optExprListPending()
{
    return exprListPending();
}

static void exprList()
{
    expr();
    if (check(COMMA))
    {
        advance();
        exprList();
    }
}
static int exprListPending()
{
    return exprPending();
}

static void uop()
{
    if (check(MINUS))
        advance();
    else if (check(PLUSPLUS))
        advance();
    else if (check(MINUSMINUS))
        advance();
    else if (check(NOT))
        advance();
    else
        failParse("unary operator");
}
static int uopPending()
{
    return check(MINUS) || check(PLUSPLUS) || check(MINUSMINUS) || check(NOT);
}

static void op()
{
    if (check(EQUALS))
        advance();
    else if (check(PLUS))
        advance();
    else if (check(MINUS))
        advance();
    else if (check(TIMES))
        advance();
    else if (check(DIVIDE))
        advance();
    else if (check(MODULUS))
        advance();
    else if (check(EXPONANT))
        advance();
    else if (check(DOT))
        advance();
    else if (check(LESS_THAN))
        advance();
    else if (check(GREATER_THAN))
        advance();
    else if (check(LESS_THAN_EQUAL))
        advance();
    else if (check(GREATER_THAN_EQUAL))
        advance();
    else if (check(EQUALSEQUALS))
        advance();
    else if (check(NOTEQUALS))
        advance();
    else if (check(LOGICAL_AND))
        advance();
    else if (check(LOGICAL_OR))
        advance();
    else if (check(BINARY_AND))
        advance();
    else if (check(BINARY_OR))
        advance();
    else
        failParse("operator");
}
static int opPending()
{
    return check(EQUALS) || check(PLUS) || check(MINUS) || check(TIMES)
        || check(DIVIDE) || check(MODULUS) || check(EXPONANT) || check(DOT)
        || check(LESS_THAN) || check(GREATER_THAN) || check(LESS_THAN_EQUAL)
        || check(GREATER_THAN_EQUAL) || check(EQUALSEQUALS) || check(NOTEQUALS)
        || check(LOGICAL_AND) || check(LOGICAL_OR) || check(BINARY_AND) || check(BINARY_OR);
}

static void optParamList()
{
    if (paramListPending())
        paramList();
}
static int optParamListPending()
{
    return paramListPending();
}

static void paramList()
{
    param();
    if (check(COMMA))
    {
        advance();
        paramList();
    }
}
static int paramListPending()
{
    return paramPending();
}

static void param()
{
    match(ID);
    if (check(EQUALS))
    {
        advance();
        expr();
    }
}
static int paramPending()
{
    return check(ID);
}

static void block()
{
    match(OBRACE);
    optStatements();
    match(CBRACE);
}
static int blockPending()
{
    return check(OBRACE);
}

static void optStatements()
{
    if (statementsPending())
        statements();
}
static int optStatementsPending()
{
    return statementsPending();
}

static void statements()
{
    statement();
    if (statementsPending())
        statements();
}
static int statementsPending()
{
    return statementPending();
}

static void statement()
{
    if (exprPending())
    {
        expr();
        match(SEMICOLON);
    }
    else if (check(VAR))
    {
        advance();
        varList();
        match(SEMICOLON);
    }
    else if (check(IF))
    {
        advance();
        match(OPAREN);
        expr();
        match(CPAREN);
        block();
        optElse();
    }
    else if (check(WHILE))
    {
        advance();
        match(OPAREN);
        expr();
        match(CPAREN);
        block();
    }
    else if (check(DO))
    {
        advance();
        block();
        match(WHILE);
        match(OPAREN);
        expr();
        match(CPAREN);
        match(SEMICOLON);
    }
    else if (check(RETURN))
    {
        advance();
        expr();
        match(SEMICOLON);
    }
    else if (check(DEFINE))
    {
        advance();
        match(ID);
        match(OPAREN);
        optParamList();
        match(CPAREN);
        block();
    }
    else
        failParse("statement");
}
static int statementPending()
{
    return exprPending() || check(VAR) || check(IF) || check(WHILE) || check(DO)
        || check(RETURN) || check(DEFINE);
}

static void optElse()
{
    if (elseStatementPending())
        elseStatement();
}
static int optElsePending()
{
    return elseStatementPending();
}

static void elseStatement()
{
    match(ELSE);
    if (blockPending())
        block();
    else if (check(IF))
    {
        advance();
        match(OPAREN);
        expr();
        match(CPAREN);
        block();
        optElse();
    }
    else
        failParse("else statement");
}
static int elseStatementPending()
{
    return check(ELSE);
}
