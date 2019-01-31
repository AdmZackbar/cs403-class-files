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
static LEXEME *cons(char *type, LEXEME *left, LEXEME *right);
// Grammar helper functions
static LEXEME *program();
static int programPending();
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
static int optVarListPending();
static LEXEME *varList();
static int varListPending();
static LEXEME *varDef();
static int varDefPending();
static LEXEME *expr();
static int exprPending();
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
static LEXEME *op();
static int opPending();
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

int main(int argc, char **argv)
{
    char *filename = parseFileArg(argc, argv);
    
    recognize(filename);

    printf("Legal\n");
    
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
    printf("Illegal\n");
    exit(-2);
}
static void recognize(char *filename)
{
    // Initialize globals
    lexer = newLEXER(filename);
    current = lex(lexer);
    
    // Highest level of program
    LEXEME *program = program();
    
    // Check if everything has been properly parsed
    match(END_OF_FILE);
}
static void failParse(char *component)
{
    printf("Incorrect structure in grammar module %s\n", component);
    printf("Illegal\n");
    exit(-404);
}
static LEXEME *cons(char *type, LEXEME *left, LEXEME *right)
{
    LEXEME *lexeme = newLEXEME(type, -1);   // TODO HANDLE LINE NUM
    setLEXEMEleft(lexeme, left);
    setLEXEMEright(lexeme, right);
    return lexeme;
}

static LEXEME *program()
{
    LEXEME *class = classDef();
    LEXEME *program = NULL;
    if(programPending())
        program = program();
    
    return cons(PROGRAM, class, program);
}
static int programPending()
{
    return classDefPending();
}

static LEXEME *classDef()
{
    LEXEME *header, *statements;
    match(CLASS);
    header = classHeader();
    match(OBRACE);
    statements = optClassStatements();
    match(CBRACE);
    
    return cons(CLASS_DEF, header, statements);
}
static int classDefPending()
{
    return check(CLASS);
}

static LEXEME *classHeader()
{
    LEXEME *id, *extendsID = NULL;
    id = match(ID);
    if (check(EXTENDS))
    {
        advance();
        extendsID = match(ID);
    }
    
    return cons(CLASS_HEADER, id, extendsID);
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
    LEXEME *statement, *statements = NULL;
    statement = classStatement();
    if (classStatementsPending())
        statements = classStatements();
    
    return cons(CLASS_STATEMENTS, statement, statements);
}
static int classStatementsPending()
{
    return classStatementPending();
}

static LEXEME *classStatement()
{
    LEXEME *accessMod, *statement;
    accessMod = accessMod();
    if (check(VAR))
    {
        advance();
        statement = varList();
        match(SEMICOLON);
    }
    else if (check(FUNCTION))
    {
        LEXEME *functionInfo, *functionName, *functionParams, *block;
        advance();
        functionName = match(ID);
        match(OPAREN);
        functionParams = optVarList();
        match(CPAREN);
        block = block();
        
        functionInfo = cons(FUNCTION_INFO, functionName, functionParams);
        statement = cons(FUNCTION_STATEMENT, functionInfo, block);
    }
    else
        failParse("class statement");
    
    return cons(CLASS_STATEMENT, accessMod, statement);
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
static int optVarListPending()
{
    return varListPending();
}

static LEXEME *varList()
{
    LEXEME *varDef, *varList = NULL;
    varDef = varDef();
    if (check(COMMA))
    {
        advance();
        varList = varList();
    }
    
    return cons(VAR_LIST, varDef, varList);
}
static int varListPending()
{
    return varDefPending();
}

static LEXEME *varDef()
{
    LEXEME *id, *expr = NULL;
    id = match(ID);
    if (check(EQUALS))
    {
        advance();
        expr = expr();
    }
    
    return cons(VAR_DEF, id, expr);
}
static int varDefPending()
{
    return check(ID);
}

static LEXEME *expr()
{
    LEXEME *unary, *operation = NULL;
    unary = unary();
    if (opPending())
    {
        LEXEME *op, *expr;
        op = op();
        expr = expr();
        operation = cons(OPERATION, op, expr);
    }
    
    return cons(EXPRESSION, unary, operation);
}
static int exprPending()
{
    return unaryPending();
}

static LEXEME *unary()
{
    if (idExprPending())
    {
        LEXEME *idExpr, *postVar;
        idExpr = idExpr;
        if (postVarPending())
            postVar = postVar();
        
        return cons(UNARY_ID, idExpr, postVar);
    }
    else if (check(INTEGER))
        return advance();
    else if (check(REAL))
        return advance();
    else if (check(STRING))
        return advance();
    else if (uopPending())
    {
        LEXEME *uop, *unary;
        uop = uop();
        unary = unary();
        return cons(UNARY_OP, uop, unary);
    }
    else if (check(OPAREN))
    {
        LEXEME *expr;
        advance();
        expr = expr();
        match(CPAREN);
        return cons(UNARY_PAREN, expr, NULL);
    }
    else if (check(NULL_WORD))
        return advance();
    else if (check(THIS))
        return advance();
    else if (check(NEW))
    {
        LEXEME *id, *exprList;
        advance();
        id = match(ID);
        match(OPAREN);
        exprList = optExprList();
        match(CPAREN);
        return cons(NEW_OBJECT, id, exprList);
    }
    failParse("unary");
}
static int unaryPending()
{
    return idExprPending() || check(INTEGER) || check(REAL) || check(STRING)
            || uopPending() || check(OPAREN) || check(NULL_WORD) || check(THIS)
            || check(NEW);
}

static LEXEME *idExpr()
{
    LEXEME *id, *postID = NULL;
    id = match(ID);
    if (check(OPAREN))
    {
        advance();
        postID = optExprList();
        match(CPAREN);
        return cons(FUNCTION_CALL, id, postID);
    }
    else if (check(OBRACKET))
    {
        advance();
        postID = expr();
        match(CBRACKET);
        return cons(ARRAY_LOOKUP, id, postID);
    }
    
    return cons(ID_EXPR, id, PostID);
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
    LEXEME *expr, *exprList = NULL;
    expr = expr();
    if (check(COMMA))
    {
        advance();
        exprList = exprList();
    }
    
    return cons(EXPR_LIST, expr, exprList);
}
static int exprListPending()
{
    return exprPending();
}

static LEXEME *uop()
{
    if (check(MINUS))
        return advance();
    else if (check(PLUSPLUS))
        return advance();
    else if (check(MINUSMINUS))
        return advance();
    else if (check(NOT))
        return advance();
    failParse("unary operator");
}
static int uopPending()
{
    return check(MINUS) || check(PLUSPLUS) || check(MINUSMINUS) || check(NOT);
}

static LEXEME *op()
{
    if (check(EQUALS))
        return advance();
    else if (check(PLUS))
        return advance();
    else if (check(MINUS))
        return advance();
    else if (check(TIMES))
        return advance();
    else if (check(DIVIDE))
        return advance();
    else if (check(MODULUS))
        return advance();
    else if (check(EXPONANT))
        return advance();
    else if (check(DOT))
        return advance();
    else if (check(LESS_THAN))
        return advance();
    else if (check(GREATER_THAN))
        return advance();
    else if (check(LESS_THAN_EQUAL))
        return advance();
    else if (check(GREATER_THAN_EQUAL))
        return advance();
    else if (check(EQUALSEQUALS))
        return advance();
    else if (check(NOTEQUALS))
        return advance();
    else if (check(LOGICAL_AND))
        return advance();
    else if (check(LOGICAL_OR))
        return advance();
    else if (check(BINARY_AND))
        return advance();
    else if (check(BINARY_OR))
        return advance();
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

static LEXEME *block()
{
    LEXEME *statements;
    match(OBRACE);
    statements = optStatements();
    match(CBRACE);
    
    return cons(BLOCK_DEF, statements, NULL);
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

static void statements()
{
    LEXEME *statement, *statements = NULL;
    statement = statement();
    if (statementsPending())
        statements = statements();
    
    return cons(STATEMENTS, statement, statements);
}
static int statementsPending()
{
    return statementPending();
}

static LEXEME *statement()
{
    LEXEME *statement;
    if (exprPending())
    {
        statement = expr();
        match(SEMICOLON);
    }
    else if (check(VAR))
    {
        advance();
        statement = varList();
        match(SEMICOLON);
    }
    else if (check(IF))
    {
        LEXEME *ifStatement, *expr, *block, *optElse;
        advance();
        match(OPAREN);
        expr = expr();
        match(CPAREN);
        block = block();
        optElse = optElse();
        
        ifStatement = cons(IF_BODY, expr, block);
        statement = cons(IF_STATEMENT, ifStatement, optElse);
    }
    else if (check(WHILE))
    {
        LEXEME *expr, *block;
        advance();
        match(OPAREN);
        expr = expr();
        match(CPAREN);
        block = block();
        
        statement = cons(WHILE_STATEMENT, expr, block);
    }
    else if (check(DO))
    {
        LEXEME *block, *expr;
        advance();
        block = block();
        match(WHILE);
        match(OPAREN);
        expr = expr();
        match(CPAREN);
        match(SEMICOLON);
        
        statement = cons(DO_WHILE_STATEMENT, block, expr);
    }
    else if (check(RETURN))
    {
        LEXEME *expr = NULL;
        advance();
        if(exprPending())
            expr = expr();
        match(SEMICOLON);
        
        statement = cons(RETURN_STATEMENT, expr, NULL);
    }
    else if (check(DEFINE))
    {
        LEXEME *header, *id, *params, *block;
        advance();
        id = match(ID);
        match(OPAREN);
        params = optVarList();
        match(CPAREN);
        block = block();
        
        header = cons(DEFINE_HEADER, id, params);
        statement = cons(DEFINE_STATEMENT, header, block);
    }
    else
        failParse("statement");
    
    return statement;
}
static int statementPending()
{
    return exprPending() || check(VAR) || check(IF) || check(WHILE) || check(DO)
        || check(RETURN) || check(DEFINE);
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
        LEXEME *block = block();
        return cons(ELSE_STATEMENT, block, NULL)
    }
    else if (check(IF))
    {
        LEXEME *ifStatement, *expr, *block, *optElse;
        advance();
        match(OPAREN);
        expr = expr();
        match(CPAREN);
        block = block();
        optElse = optElse();
        
        ifStatement = cons(IF_BODY, expr, block);
        return cons(IF_STATEMENT, ifStatement, optElse);
    }
    else
        failParse("else statement");
}
static int elseStatementPending()
{
    return check(ELSE);
}
