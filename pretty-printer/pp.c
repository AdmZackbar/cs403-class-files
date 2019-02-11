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
static void printExpr(LEXEME *tree, int indent);

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
    char *type = getTypeLEXEME(tree);
    if (isPrimative(tree) || isOperator(tree) || getTypeLEXEME(tree) == ID || isAccessMod(tree)) displayLEXEME(stdout, tree);
    else if (type == PROG)   printProgram(tree, indent);
    else if (type == CLASS_DEF) printClassDef(tree, indent);
    else if (type == CLASS_HEADER)  printClassHeader(tree, indent);
    else if (type == CLASS_STATEMENTS)  printClassStatements(tree, indent);
    else if (type == CLASS_STATEMENT)   printClassStatement(tree, indent);
    else if (type == FUNCTION_STATEMENT)    printFunctionStatement(tree, indent);
    else if (type == VAR_DECL)  printVarDecl(tree, indent);
    else if (type == VAR_LIST)  printVarList(tree, indent);
    else if (type == VAR_DEF)   printVarDef(tree, indent);
    else if (type == EXPR)  printExpr(tree, indent);
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
    printf("Class ");
    displayLEXEME(car(tree));   // ID - classname
    if (cdr(tree) != NULL))
    {
        printf(" extends ");
        displayLEXEME(cdr(tree));
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
    pp(cdr(tree), indent);  // var declaration or function statement
}

static void printFunctionStatement(LEXEME *tree, int indent)
{
    printf("function ");
    pp(car(cdr(tree)), indent);  // ID - function name
    printf("(");
    LEXEME *varList = cdr(cdr(tree));
    if (varList)    pp(varList, indent);  // Var list
    printf(")\n");
    pp(car(tree), indent);  // Block
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

static void printExpr(LEXEME *tree, int indent)
{
    pp(car(tree), indent);  // Unary
    if (cdr(tree))
    {
        printf(" ");
        pp(car(cdr(tree)), indent); // Operator
        printf(" ");
        pp(cdr(cdr(tree)), indent); // Expr
    }
}
