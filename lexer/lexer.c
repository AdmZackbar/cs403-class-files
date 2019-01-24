#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "lexer.h"
#include "scanner.h"
#include "lexeme.h"
#include "types.h"
#include "stringBuffer.h"

/*
 * Author:  Zach Wassynger
 * Date:    1/22/2019
 * Purpose: Part of the designer programming language project of CS 403.
 *          Scans a given file to determine its "lexemes".
 */

struct lexer
{
    FILE *fp;
    int lineNum;
};

static LEXEME *lexNumber(LEXER *lexer, int ch);
static LEXEME *lexString(LEXER *lexer);
static LEXEME *lexWord(LEXER *lexer, int ch);
static LEXEME *lexID(char *word);
static int iswordch(int ch);
static int wordIs(char *reserved, char *word, int wordLength);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Incorrect number of parameters, expected 2, got %d.\n", argc);
        printf("Correct usage: %s <filepath>\n", argv[0]);
        return -1;
    }
    
    LEXER *lexer = newLEXER(argv[1]);
    
    LEXEME *lexeme = lex(lexer);
    while (getTypeLEXEME(lexeme) != END_OF_FILE)
    {
        printLEXEME(stdout, lexeme);
        printf("\n");
        
        lexeme = lex(lexer);
    }

    return 0;
}

LEXER *newLEXER(char *filename)
{
    LEXER *l = malloc(sizeof(LEXER));
    assert(l != 0);
    
    l->fp = fopen(filename, "rw");
    assert(l->fp != 0);
    l->lineNum = 1;
    
    return l;
}

LEXEME *lex(LEXER *lexer)
{
    int ch;

    ch = readChar(lexer->fp, &lexer->lineNum);
    
    if (ch == EOF)
        return newLEXEME(END_OF_FILE, lexer->lineNum);
    if (ch == ';')
        return newLEXEME(SEMICOLON, lexer->lineNum);
    if (ch == ',')
        return newLEXEME(COMMA, lexer->lineNum);
    if (ch == '{')
        return newLEXEME(OBRACE, lexer->lineNum);
    if (ch == '}')
        return newLEXEME(CBRACE, lexer->lineNum);
    if (ch == '(')
        return newLEXEME(OPAREN, lexer->lineNum);
    if (ch == ')')
        return newLEXEME(CPAREN, lexer->lineNum);
    if (ch == '[')
        return newLEXEME(OBRACKET, lexer->lineNum);
    if (ch == ']')
        return newLEXEME(CBRACKET, lexer->lineNum);
    if (isdigit(ch) || ch == '.')
        return lexNumber(lexer->fp, ch);
    if (ch == '"')
        return lexString(lexer->fp);
    if (isalpha(ch))
        return lexWord(lexer->fp, ch);
    
    char *errorChar = malloc(sizeof(char) * 2);
    errorChar[0] = ch;
    errorChar[1] = '\0';
    return newLEXEMEstring(PARSE_ERROR, errorChar, lexer->lineNum);
}

static LEXEME *lexNumber(LEXER *lexer, int ch)
{
    STRING_BUFFER *buffer = newSTRINGBUFFER();
    int isReal = 0;
    
    while(ch != EOF && (isdigit(ch) || ch == '.'))
    {
        addCharBUFFER(buffer, ch);
        if(ch == '.' && isReal)
            return newLEXEMEstring(BAD_NUM, returnStringBUFFER(buffer), lexer->lineNum);
        if(ch == '.')
            isReal = 1;
        ch = readChar(lexer->fp, &lexer->lineNum);
    }
    pushbackChar(lexer->fp, ch);
    if(isReal)
        return newLEXEMEdouble(atof(returnStringBUFFER(buffer)), lexer->lineNum);
    return newLEXEMEint(atoi(returnStringBUFFER(buffer)), lexer->lineNum);
}

static LEXEME *lexString(LEXER *lexer)
{
    STRING_BUFFER *buffer = newSTRINGBUFFER();
    
    int ch = readChar(lexer->fp, &lexer->lineNum);
    while(ch != EOF && ch != '"')
    {
        addCharBUFFER(buffer, ch);
        ch = readChar(lexer->fp, &lexer->lineNum);
    }
    
    return newLEXEMEstring(STRING, returnStringBUFFER(buffer), lexer->lineNum);
}

static LEXEME *lexWord(LEXER *lexer, int ch)
{
    STRING_BUFFER *buffer = newSTRINGBUFFER();
    
    while(ch != EOF && iswordch(ch))
    {
        addCharBUFFER(buffer, ch);
        ch = readChar(lexer->fp, &lexer->lineNum);
    }

    int wordSize = getLengthBUFFER(buffer);
    char *word = returnStringBUFFER(buffer);
    if(wordIs(VAR, word, wordSize))
        return newLEXEME(VAR, lexer->lineNum);
    if(wordIs(FUNCTION, word, wordSize))
        return newLEXEME(FUNCTION, lexer->lineNum);
    if(wordIs(DEFINE, word, wordSize))
        return newLEXEME(DEFINE, lexer->lineNum);
    if(wordIs(CLASS, word, wordSize))
        return newLEXEME(CLASS, lexer->lineNum);
    if(wordIs(PUBLIC, word, wordSize))
        return newLEXEME(PUBLIC, lexer->lineNum);
    if(wordIs(PRIVATE, word, wordSize))
        return newLEXEME(PRIVATE, lexer->lineNum);
    if(wordIs(PROTECTED, word, wordSize))
        return newLEXEME(PROTECTED, lexer->lineNum);
    if(wordIs(NULL_WORD, word, wordSize))
        return newLEXEME(NULL_WORD, lexer->lineNum);
    if(wordIs(THIS, word, wordSize))
        return newLEXEME(THIS, lexer->lineNum);
    if(wordIs(NEW, word, wordSize))
        return newLEXEME(NEW, lexer->lineNum);
    if(wordIs(IF, word, wordSize))
        return newLEXEME(IF, lexer->lineNum);
    if(wordIs(ELSE, word, wordSize))
        return newLEXEME(ELSE, lexer->lineNum);
    if(wordIs(WHILE, word, wordSize))
        return newLEXEME(WHILE, lexer->lineNum);
    if(wordIs(DO, word, wordSize))
        return newLEXEME(DO, lexer->lineNum);
    if(wordIs(RETURN, word, wordSize))
        return newLEXEME(RETURN, lexer->lineNum);
    
    return lexID(word, lexer->lineNum);
}

static LEXEME *lexID(char *word, int lineNum)
{
    return newLEXEMEstring(ID, word, lineNum);
}

static int iswordch(int ch)
{
    return isalpha(ch) || ch == '_';
}

static int wordIs(char *reserved, char *word, int wordLength)
{
    for(int i=0; i<wordLength; i++)
    {
        word[i] = tolower(word[i]);
    }
    return !strcmp(reserved, word);
}
