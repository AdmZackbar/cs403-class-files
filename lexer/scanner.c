#include <stdio.h>
#include <ctype.h>  // For isspace
#include "scanner.h"

static int skipWhitespace(FILE *fp);
static void skipLineComment(FILE *fp);
static int skipBlockComment(FILE *fp);
static int isNewLine(FILE *fp, int ch);

char readChar(FILE *fp, int *lineNum)
{
    *lineNum += skipWhitespace(fp);

    return fgetc(fp);
}

void pushbackChar(FILE *fp, int ch)
{
    ungetc(ch, fp);
}

/*
 * Reads in and ignores all whitespace until a character is found.
 * If the character is part of a comment, it too is ignored. This process
 * ends with the finding of a character that is not in a comment.
 */
static int skipWhitespace(FILE *fp)
{
    int ch, numNewLines = 0;

    //Read characters until non-whitespace is found
    while((ch = fgetc(fp)) != EOF && isspace(ch))
    {
        if (ch == '\n')
            numNewLines++;
    }
    
    if (ch == '/')
    {
        ch = fgetc(fp);
        if(ch == '/') // is a line comment
        {
            skipLineComment(fp);
            return 1 + numNewLines + skipWhitespace(fp);
        }
        else if (ch == '*') // is a block comment
        {
            numNewLines += skipBlockComment(fp);
            return numNewLines + skipWhitespace(fp);
        }
        // not a comment: need to re-add those characters
        ungetc(ch, fp);
        ungetc('/', fp);
        return numNewLines; // return early since we are re-adding the character later
    }

    if(ch != EOF)
        ungetc(ch,fp);
        
    return numNewLines;
}

/*
 * Reads in and ignores all the characters until a newline is found.
 * The newline character(s) is consumed at the end.
 */
static void skipLineComment(FILE *fp)
{
    int ch = fgetc(fp);
    while(!isNewLine(fp, ch))
        ch = fgetc(fp);
}

/*
 * Reads in and ignores all characters until the end string is found.
 * The end string is consumed at the end.
 */
static int skipBlockComment(FILE *fp)
{
    int ch = fgetc(fp), numNewLines = 0;
    while(ch != EOF)
    {
        if (ch == '*')
        {
            ch = fgetc(fp);
            if (ch == '/')
                break;
        }
        else if (ch == '\n')
        {
            numNewLines++;
        }
        ch = fgetc(fp);
    }
    
    return numNewLines;
}

/*
 * Checks if the given character is part of a newline. Checks for the
 * Windows newline(\r\n), the Linux newline(\n), and the OS X newline
 * (\r). Returns 1(true) if a newline is found, 0 if otherwise. The
 * passed in character is not re-added to the file under any
 * circumstances.
 */
static int isNewLine(FILE *fp, int ch)
{
    if (ch == '\r')
    {
        ch = fgetc(fp);
        if (ch != '\n')
            ungetc(ch, fp);
        return 1;
    }
    else if(ch == '\n')
        return 1;
    return 0;
}
