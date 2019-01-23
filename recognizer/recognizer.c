#include <stdio.h>
#include "lexer.h"
#include "lexeme.h"
#include "types.h"

// Normal helper functions
static char *parseFileArg(int argc, char **argv);
static LEXEME *advance();
static int check(char *type);
static LEXEME *match(char *type);
static void recognize(char *filename);
// Grammar helper functions
static void program();
static int programPending();
static void classDef();
static int classDefPending();

LEXER *lexer;
LEXEME *current;

int main(int argc, char **argv)
{
    char *filename = parseFileArg(argc, argv);
    
    recognize(filename);
    
    return 0;
}

static char *parseFileArg(int argv, char **argv)
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
        
    printf("SYNTAX ERROR: expected %s got %s", type, getTypeLEXEME(current));
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