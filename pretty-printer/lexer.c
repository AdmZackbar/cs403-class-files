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
static LEXEME *lexID(char *word, int lineNum);
static int iswordch(int ch);
static int wordIs(char *reserved, char *word);
static char *strToLower(char *str, int strLen);

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

    skipWhitespace(lexer->fp, &lexer->lineNum);

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
    if (ch == '=')
    {
        ch = readChar(lexer->fp, &lexer->lineNum);
        switch (ch)
        {
            case '=':   return newLEXEME(EQUALSEQUALS, lexer->lineNum);
            default:
                pushbackChar(lexer->fp, ch, &lexer->lineNum);
                return newLEXEME(EQUALS, lexer->lineNum);
        }
    }
    if (ch == '+')
    {
        ch = readChar(lexer->fp, &lexer->lineNum);
        switch (ch)
        {
            case '+':   return newLEXEME(PLUSPLUS, lexer->lineNum);
            default:
                pushbackChar(lexer->fp, ch, &lexer->lineNum);
                return newLEXEME(PLUS, lexer->lineNum);
        }
    }
    if (ch == '-')
    {
        ch = readChar(lexer->fp, &lexer->lineNum);
        switch (ch)
        {
            case '-':   return newLEXEME(MINUSMINUS, lexer->lineNum);
            default:
                pushbackChar(lexer->fp, ch, &lexer->lineNum);
                return newLEXEME(MINUS, lexer->lineNum);
        }
    }
    if (ch == '*')
        return newLEXEME(TIMES, lexer->lineNum);
    if (ch == '/')
        return newLEXEME(DIVIDE, lexer->lineNum);
    if (ch == '%')
        return newLEXEME(MODULUS, lexer->lineNum);
    if (ch == '%')
        return newLEXEME(MODULUS, lexer->lineNum);
    if (ch == '^')
        return newLEXEME(EXPONANT, lexer->lineNum);
    if (ch == '<')
    {
        ch = readChar(lexer->fp, &lexer->lineNum);
        switch (ch)
        {
            case '=':   return newLEXEME(LESS_THAN_EQUAL, lexer->lineNum);
            default:
                pushbackChar(lexer->fp, ch, &lexer->lineNum);
                return newLEXEME(LESS_THAN, lexer->lineNum);
        }
    }
    if (ch == '>')
    {
        ch = readChar(lexer->fp, &lexer->lineNum);
        switch (ch)
        {
            case '=':   return newLEXEME(GREATER_THAN_EQUAL, lexer->lineNum);
            default:
                pushbackChar(lexer->fp, ch, &lexer->lineNum);
                return newLEXEME(GREATER_THAN, lexer->lineNum);
        }
    }
    if (ch == '&')
    {
        ch = readChar(lexer->fp, &lexer->lineNum);
        switch (ch)
        {
            case '&':   return newLEXEME(LOGICAL_AND, lexer->lineNum);
            default:
                pushbackChar(lexer->fp, ch, &lexer->lineNum);
                return newLEXEME(BINARY_AND, lexer->lineNum);
        }
    }
    if (ch == '|')
    {
        ch = readChar(lexer->fp, &lexer->lineNum);
        switch (ch)
        {
            case '|':   return newLEXEME(LOGICAL_OR, lexer->lineNum);
            default:
                pushbackChar(lexer->fp, ch, &lexer->lineNum);
                return newLEXEME(BINARY_OR, lexer->lineNum);
        }
    }
    if (ch == '.')
        return newLEXEME(DOT, lexer->lineNum);
    if (ch == '!')
    {
        ch = readChar(lexer->fp, &lexer->lineNum);
        switch (ch)
        {
            case '=':   return newLEXEME(NOTEQUALS, lexer->lineNum);
            default:
                pushbackChar(lexer->fp, ch, &lexer->lineNum);
                return newLEXEME(NOT, lexer->lineNum);
        }
    }
    if (isdigit(ch))
        return lexNumber(lexer, ch);
    if (ch == '"')
        return lexString(lexer);
    if (iswordch(ch))
        return lexWord(lexer, ch);
    
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
    pushbackChar(lexer->fp, ch, &lexer->lineNum);
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
    //pushbackChar(lexer->fp, ch, &lexer->lineNum); - not needed as we do not want the quotation mark
    
    return newLEXEMEstring(STRING, returnStringBUFFER(buffer), lexer->lineNum);
}

static LEXEME *lexWord(LEXER *lexer, int ch)
{
    STRING_BUFFER *buffer = newSTRINGBUFFER();
    
    while (ch != EOF && iswordch(ch))
    {
        addCharBUFFER(buffer, ch);
        ch = readChar(lexer->fp, &lexer->lineNum);
    }
    pushbackChar(lexer->fp, ch, &lexer->lineNum);

    int wordSize = getLengthBUFFER(buffer);
    char *originalWord = returnStringBUFFER(buffer);
    char *word = strToLower(originalWord, wordSize);
    if (wordIs(VAR, word))
        return newLEXEME(VAR, lexer->lineNum);
    if (wordIs(FUNCTION, word))
        return newLEXEME(FUNCTION, lexer->lineNum);
    //if (wordIs(DEFINE, word))
    //    return newLEXEME(DEFINE, lexer->lineNum);
    if (wordIs(CLASS, word))
        return newLEXEME(CLASS, lexer->lineNum);
    if (wordIs(EXTENDS, word))
        return newLEXEME(EXTENDS, lexer->lineNum);
    if (wordIs(PUBLIC, word))
        return newLEXEME(PUBLIC, lexer->lineNum);
    if (wordIs(PRIVATE, word))
        return newLEXEME(PRIVATE, lexer->lineNum);
    if (wordIs(PROTECTED, word))
        return newLEXEME(PROTECTED, lexer->lineNum);
    if (wordIs(NULL_WORD, word))
        return newLEXEME(NULL_WORD, lexer->lineNum);
    if (wordIs(THIS, word))
        return newLEXEME(THIS, lexer->lineNum);
    if (wordIs(NEW, word))
        return newLEXEME(NEW, lexer->lineNum);
    if (wordIs(IF, word))
        return newLEXEME(IF, lexer->lineNum);
    if (wordIs(ELSE, word))
        return newLEXEME(ELSE, lexer->lineNum);
    if (wordIs(WHILE, word))
        return newLEXEME(WHILE, lexer->lineNum);
    if (wordIs(DO, word))
        return newLEXEME(DO, lexer->lineNum);
    if (wordIs(LAMBDA, word))
        return newLEXEME(LAMBDA, lexer->lineNum);
    if (wordIs(RETURN, word))
        return newLEXEME(RETURN, lexer->lineNum);
    
    free(word);
    return lexID(originalWord, lexer->lineNum);
}

static LEXEME *lexID(char *word, int lineNum)
{
    return newLEXEMEstring(ID, word, lineNum);
}

static int iswordch(int ch)
{
    return isalpha(ch) || ch == '_' || isdigit(ch);
}

static int wordIs(char *reserved, char *word)
{
    return !strcmp(reserved, word);
}

static char *strToLower(char *str, int strLen)
{
    char *word = malloc(sizeof(char) * (strLen+1));
    for(int i=0; i<strLen; i++)
    {
        word[i] = tolower(str[i]);
    }
    word[strLen] = '\0';
    return word;
}
