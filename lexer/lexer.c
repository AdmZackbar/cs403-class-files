#include "scanner.h"
#include "lexeme.h"
#include "types.h"

static LEXEME *lex(FILE *fp);

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
    while (lexeme != NULL)
    {
        printLEXEME(stdout, lexeme);
        printf("\n");
        
        lexeme = lex(fp);
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
        return lexReserved(fp, ch);
    
    return newLEXEMEerror(PARSE_ERROR, ch);
}

static LEXEME *lexNumber(FILE *fp, int ch)
{
    STRING_BUFFER *buffer = newSTRINGBUFFER();
    int isReal = 0;
    
    while(ch != EOF && (isdigit(ch) || ch == '.')
    {
        buffer.addCharBUFFER(buffer, ch);
        if(ch == '.' && isReal)
            return newLEXEME(BAD_NUM, buffer.getStringBUFFER());
        if(ch == '.')
            isReal = 1;
        ch = readChar(fp);
    }
    pushbackChar(fp, ch);
    if(isReal)
        return newLEXEME(REAL, atof(returnStringBUFFER(buffer)));
    return newLEXEME(INT, atoi(returnStringBUFFER(buffer)));
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
    
    return newLEXEME(STRING, returnStringBUFFER(buffer));
}

static LEXEME *lexReserved(FILE *fp, int ch)
{
    STRING_BUFFER *buffer = newSTRINGBUFFER();
    
    if (ch == 'v')
}

static LEXEME *lexID(FILE *fp, int ch)
{
    STRING_BUFFER *buffer = newSTRINGBUFFER();
    
    while(isalpha(ch) || isdigit(ch) || ch == '_' || ch == '-')
    {
        addCharBUFFER(buffer, ch);
        ch = readChar(fp);
    }
    
    return newLEXEME(ID, returnStringBUFFER(buffer));
}
