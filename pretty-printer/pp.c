#include <stdio.h>  // for printf()
#include <stdlib.h> // for exit()
#include "pp.h"
#include "parser.h"
#include "lexeme.h"
#include "types.h"

static char *parseFileArg(int argc, char **argv);
static void printIndent(int indent);

static void printProgram(LEXEME *tree, int indent);
static void printClassDef(LEXEME *tree, int indent);
static void printClassHeader(LEXEME *tree, int indent);
static void printClassStatements(LEXEME *tree, int indent);
static void printClassStatement(LEXEME *tree, int indent);
static void printFunctionStatement(LEXEME *tree, int indent);
static void printVarDecl(LEXEME *tree, int indent);
static void printVarList(LEXEME *tree, int indent);
static void printVarDef(LEXEME *tree, int indent);
//static void printExpr(LEXEME *tree, int indent);
static void printOp(LEXEME *tree, int indent);
static void printStatements(LEXEME *tree, int indent);
static void printIf(LEXEME *tree, int indent);
static void printElse(LEXEME *tree, int indent);
static void printWhile(LEXEME *tree, int indent);
static void printDoWhile(LEXEME *tree, int indent);
static void printReturn(LEXEME *tree, int indent);
static void printUnaryID(LEXEME *tree, int indent);
static void printFunctionCall(LEXEME *tree, int indent);
static void printArrayLookup(LEXEME *tree, int indent);
static void printExprList(LEXEME *tree, int indent);

int indentSpaces = 4;   // Number of spaces per tab(indent)

int main(int argc, char **argv)
{
    char *filename = parseFileArg(argc, argv);
    
    LEXEME *program = parse(filename);
    
    pp(program, 0);
    
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

static void printIndent(int indent)
{
    while (indent > 0) {
        printf(" ");
        indent--;
    }
}

void pp(LEXEME *tree, int indent)
{
    if (tree == NULL)
    {
        printf("Got NULL tree in pp\n");
        return;
    }
    char *type = getTypeLEXEME(tree);
    if (isPrimative(tree) || getTypeLEXEME(tree) == ID || isAccessMod(tree) || isReserved(tree)) displayLEXEME(stdout, tree);
    else if (type == PROG)   printProgram(tree, indent);
    else if (type == CLASS_DEF) printClassDef(tree, indent);
    else if (type == CLASS_HEADER)  printClassHeader(tree, indent);
    else if (type == CLASS_STATEMENTS)  printClassStatements(tree, indent);
    else if (type == CLASS_STATEMENT)   printClassStatement(tree, indent);
    else if (type == FUNCTION_STATEMENT)    printFunctionStatement(tree, indent);
    else if (type == VAR_DECL)  printVarDecl(tree, indent);
    else if (type == VAR_LIST)  printVarList(tree, indent);
    else if (type == VAR_DEF)   printVarDef(tree, indent);
    //else if (type == EXPR)  printExpr(tree, indent);
    else if (isOperator(tree))  printOp(tree, indent);
    else if (type == STATEMENTS)    printStatements(tree, indent);
    else if (type == IF_STATEMENT)  printIf(tree, indent);
    else if (type == ELSE_STATEMENT)    printElse(tree, indent);
    else if (type == WHILE_STATEMENT)   printWhile(tree, indent);
    else if (type == DO_WHILE_STATEMENT)    printDoWhile(tree, indent);
    else if (type == RETURN_STATEMENT)  printReturn(tree, indent);
    else if (type == UNARY_ID)  printUnaryID(tree, indent);
    else if (type == FUNCTION_CALL) printFunctionCall(tree, indent);
    else if (type == ARRAY_LOOKUP)  printArrayLookup(tree, indent);
    else if (type == EXPR_LIST) printExprList(tree, indent);
}

static void printProgram(LEXEME *tree, int indent)
{
    pp(car(tree), indent);  // ClassDef
    if(cdr(tree))   pp(cdr(tree), indent);  // Program
}

static void printClassDef(LEXEME *tree, int indent)
{
    pp(car(tree), indent);  // Class header
    printf("\n");
    printIndent(indent);
    printf("{\n");
    if (cdr(tree))  pp(cdr(tree), indent+indentSpaces); // Class statements
    printf("}\n");
}

static void printClassHeader(LEXEME *tree, int indent)
{
    printIndent(indent);
    printf("class ");
    pp(car(tree), indent);   // ID - class name
    if (cdr(tree))
    {
        printf(" extends ");
        pp(cdr(tree), indent);  // ID - base class name
    }
}

static void printClassStatements(LEXEME *tree, int indent)
{
    pp(car(tree), indent);  // Class statement
    if (cdr(tree))  pp(cdr(tree), indent);  // Class statements
}

static void printClassStatement(LEXEME *tree, int indent)
{
    printIndent(indent);
    if (car(tree))
    {
        pp(car(tree), indent);  // Access Mod
        printf(" ");
    }
    pp(cdr(tree), indent);  // Var declaration or function statement
}

static void printFunctionStatement(LEXEME *tree, int indent)
{
    printf("function ");
    pp(car(cdr(tree)), indent);  // ID - function name
    printf("(");
    LEXEME *varList = cdr(cdr(tree));
    if (varList)    pp(varList, indent);  // Var list
    printf(")\n");
    printIndent(indent);
    printf("{\n");
    if (car(tree))  pp(car(tree), indent+indentSpaces); // Statements
    printIndent(indent);
    printf("}\n");
}

static void printVarDecl(LEXEME *tree, int indent)
{
    printf("var ");
    pp(car(tree), indent);  // Var list
    printf(";\n");
}

static void printVarList(LEXEME *tree, int indent)
{
    pp(car(tree), indent);  // Var def
    if (cdr(tree))
    {
        printf(", ");
        pp(cdr(tree), indent);  // Var list
    }
}

static void printVarDef(LEXEME *tree, int indent)
{
    pp(car(tree), indent);  // ID - var name
    if (cdr(tree))
    {
        printf(" = ");
        pp(cdr(tree), indent);  // Expr
    }
}

//static void printExpr(LEXEME *tree, int indent)
//{
//    pp(car(tree), indent);  // Unary
//    if (cdr(tree))
//    {
//        printf(" ");
//        pp(car(cdr(tree)), indent); // Operator
//        printf(" ");
//        pp(cdr(cdr(tree)), indent); // Expr
//    }
//}

static void printOp(LEXEME *tree, int indent)
{
    pp(car(tree), indent);  // Unary/Operator
    if (tree != DOT)    printf(" ");
    displayLEXEME(stdout, tree);    // Operator
    if (tree != DOT)    printf(" ");
    pp(cdr(tree), indent);  // Unary/Operator
}

static void printStatements(LEXEME *tree, int indent)
{
    printIndent(indent);
    pp(car(tree), indent);  // Statement - if, else, etc...
    if (isOperator(car(tree)))  printf(";\n");  // After expressions
    //printf("\n");
    if (cdr(tree))  pp(cdr(tree), indent);
}

static void printIf(LEXEME *tree, int indent)
{
    printf("if (");
    pp(car(cdr(tree)), indent); // If conditional - expr
    printf(")\n");
    printIndent(indent);
    printf("{\n");
    printIndent(indent+indentSpaces);
    pp(cdr(cdr(tree)), indent+indentSpaces);    // If block - statements
    printIndent(indent);
    printf("}\n");
    if (car(tree))  pp(car(tree), indent);  // Else
}

static void printElse(LEXEME *tree, int indent)
{
    printIndent(indent);
    if (getTypeLEXEME(car(tree)) == IF_STATEMENT)
    {
        printf("else ");
        pp(car(tree), indent);  // If statement
    }
    else
    {
        printf("else\n{");
        pp(car(tree), indent+indentSpaces);    // Block - statements
        printIndent(indent);
        printf("}\n");
    }
}

static void printWhile(LEXEME *tree, int indent)
{
    printf("while (");
    pp(car(tree), indent);  // While conditional - expr
    printf(")\n");
    printIndent(indent);
    printf("{\n");
    pp(cdr(tree), indent+indentSpaces);    // Body - statements
    printIndent(indent);
    printf("}\n");
}

static void printDoWhile(LEXEME *tree, int indent)
{
    printf("do\n");
    printIndent(indent);
    printf("{\n");
    pp(car(tree), indent+indentSpaces); // Block - statements
    printIndent(indent);
    printf("} while (");
    pp(cdr(tree), indent);  // While conditional - expr
    printf(")\n");
}

static void printReturn(LEXEME *tree, int indent)
{
    printf("return");
    if (car(tree))
    {
        printf(" ");
        pp(car(tree), indent);  // Return statement - expr
        printf(";\n");
    }
    else    printf(";\n");
}

static void printUnaryID(LEXEME *tree, int indent)
{
    pp(car(tree), indent);  // IdExpr
    pp(cdr(tree), indent);  // Postvar
}

static void printFunctionCall(LEXEME *tree, int indent)
{
    pp(car(tree), indent);  // ID
    printf("(");
    if (cdr(tree))  pp(cdr(tree));  // ExprList
    printf(")");
}

static void printArrayLookup(LEXEME *tree, int indent)
{
    pp(car(tree), indent);  // ID
    printf("[");
    pp(cdr(tree));  // Expr
    printf("]");
}

static void printExprList(LEXEME *tree, int indent)
{
    pp(car(tree), indent);  // Expr
    if (cdr(tree))
    {
        printf(", ");
        pp(cdr(tree), indent);  // ExprList
    }
}
