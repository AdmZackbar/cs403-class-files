#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"
#include "lexeme.h"
#include "types.h"
#include "stringBuffer.h"

static LEXEME *lex(FILE *fp);
static LEXEME *lexNumber(FILE *fp, int ch);
static LEXEME *lexString(FILE *fp);
static LEXEME *lexWord(FILE *fp, int ch);
static LEXEME *lexID(char *word);

static int wordIs(char *reserved, char *word, int wordLength);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Incorrect number of parameters, expected 2, got %d.\n", argc);
        printf("Correct usage: %s <filepath>\n", argv[0]);
        return -1;
    }
    
    FILE *fp = fopen(argv[1], "rw");
    
    if (!fp)
    {
        printf("Could not open file %s\n", argv[1]);
        return -2;
    }
    
    LEXEME *lexeme = lex(fp);
    while (lexeme != NULL && !isErrorLEXEME(lexeme))
    {
        printLEXEME(stdout, lexeme);
        printf("\n");
        
        lexeme = lex(fp);
    }

    if(isErrorLEXEME(lexeme))
    {
        printLEXEME(stdout, lexeme);
        return -3;
    }

    return 0;
}

static LEXEME *lex(FILE *fp)
{
    int ch;

    ch = readChar(fp);
    
    if (ch == EOF)
        return NULL;
    if (ch == '{')
        return newLEXEME(OBRACE);
    if (ch == '}')
        return newLEXEME(CBRACE);
    if (ch == '(')
        return newLEXEME(OPAREN);
    if (ch == ')')
        return newLEXEME(CPAREN);
    if (ch == '[')
        return newLEXEME(OBRACKET);
    if (ch == ']')
        return newLEXEME(CBRACKET);
    if (isdigit(ch) || ch == '.')
        return lexNumber(fp, ch);
    if (ch == '"')
        return lexString(fp);
    if (isalpha(ch))
        return lexWord(fp, ch);
    
    return newLEXEMEerror(PARSE_ERROR, ch);
}

static LEXEME *lexNumber(FILE *fp, int ch)
{
    STRING_BUFFER *buffer = newSTRINGBUFFER();
    int isReal = 0;
    
    while(ch != EOF && (isdigit(ch) || ch == '.'))
    {
        addCharBUFFER(buffer, ch);
        if(ch == '.' && isReal)
            return newLEXEMEvalue(BAD_NUM, returnStringBUFFER(buffer));
        if(ch == '.')
            isReal = 1;
        ch = readChar(fp);
    }
    pushbackChar(fp, ch);
    if(isReal)
        return newLEXEMEdouble(REAL, atof(returnStringBUFFER(buffer)));
    return newLEXEMEint(INTEGER, atoi(returnStringBUFFER(buffer)));
}

static LEXEME *lexString(FILE *fp)
{
    STRING_BUFFER *buffer = newSTRINGBUFFER();
    
    int ch = readChar(fp);
    while(ch != EOF && ch != '"')
    {
        addCharBUFFER(buffer, ch);
        ch = readChar(fp);
    }
    
    return newLEXEMEvalue(STRING, returnStringBUFFER(buffer));
}

static LEXEME *lexWord(FILE *fp, int ch)
{
    STRING_BUFFER *buffer = newSTRINGBUFFER();
    
    while(!isspace(ch))
    {
        addCharBUFFER(buffer, ch);
        ch = readChar(fp);
    }

    int wordSize = getLengthBUFFER(buffer);
    char *word = returnStringBUFFER(buffer);
    if(wordIs(VAR, word, wordSize))
        return newLEXEME(VAR);
    if(wordIs(FUNCTION, word, wordSize))
        return newLEXEME(FUNCTION);
    if(wordIs(DEFINE, word, wordSize))
        return newLEXEME(DEFINE);
    
    return lexID(word);
}

static LEXEME *lexID(char *word)
{
    return newLEXEMEvalue(ID, word);
}

static int wordIs(char *reserved, char *word, int wordLength)
{
    for(int i=0; i<wordLength; i++)
    {
        word[i] = tolower(word[i]);
    }
    return !strcmp(reserved, word);
}
