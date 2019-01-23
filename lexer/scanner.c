#include <stdio.h>
#include <ctype.h>  // For isspace
#include "scanner.h"

static void skipWhitespace(FILE *fp);

char readChar(FILE *fp)
{
    skipWhitespace(fp);

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
static void skipWhitespace(FILE *fp)
{
    int ch;

    //Read characters until non-whitespace is found
    while((ch = fgetc(fp)) != EOF && isspace(ch))
        continue;
    
    if (ch == '/')
    {
        ch = fgetc(fp);
        if(ch == '/')    // is a line comment
        {
            skipLineComment(fp);
            skipWhitespace(fp);
        }
        else if (ch == '*')    // is a block comment
        {
            skipBlockComment(fp);
            skipWhitespace(fp);
        }
        else    // not a comment: need to re-add those characters
        {
            ungetc(ch, fp);
            ungetc('/', fp);
            return;    // return early since we are re-adding the character later
        }
    }

    if(ch != EOF)
        ungetc(ch,fp);
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
 * Reads in and ignores all characters until the string "*/" is found.
 * The end string(*/) is consumed at the end.
 */
static void skipLineComment(FILE *fp)
{
    int ch = fgetc(fp);
    while(ch != EOF)
    {
        if (ch == '*')
        {
            ch = fgetc(fp);
            if (ch == '/')
                break;
        }
        ch = fgetc(fp);
    }
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
