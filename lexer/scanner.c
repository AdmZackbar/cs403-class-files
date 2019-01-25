#include <stdio.h>
#include <ctype.h>  // For isspace
#include "scanner.h"

/*
 * Author:  Zach Wassynger
 * Date:    1/22/2019
 * Purpose: Part of the designer programming language project of CS 403.
 *          Can read in a file one character at a time, skipping whitespace and
 *          comments as needed.
 */

static void checkComment(FILE *fp, int *lineNum);
static void skipLineComment(FILE *fp, int *lineNum);
static void skipBlockComment(FILE *fp, int *lineNum);

char readChar(FILE *fp, int *lineNum)
{
    checkComment(fp, lineNum);
    return fgetc(fp);
}

void pushbackChar(FILE *fp, int ch, int *lineNum)
{
    if (ch == '\n')
        (*lineNum)--;
    ungetc(ch, fp);
}

/*
 * Reads in and ignores all whitespace until a character is found.
 * If the character is part of a comment, it too is ignored. This process
 * ends with the finding of a character that is not in a comment.
 */
void skipWhitespace(FILE *fp, int *lineNum)
{
    int ch;

    //Read characters until non-whitespace is found
    while((ch = fgetc(fp)) != EOF && isspace(ch))
    {
        if (ch == '\n')
            (*lineNum)++;
    }

    if(ch != EOF)
        ungetc(ch,fp);
}

static void checkComment(FILE *fp, int *lineNum)
{
    int ch = fgetc(fp);
    
    if (ch == '/')
    {
        ch = fgetc(fp);
        if(ch == '/') // is a line comment
            skipLineComment(fp, lineNum);
        else if (ch == '*') // is a block comment
            skipBlockComment(fp, lineNum);
        else
        {
            // not a comment: need to re-add those characters
            ungetc(ch, fp);
            ungetc('/', fp);
        }
    }
    else if (ch != EOF)
        ungetc(ch, fp);
}

/*
 * Reads in and ignores all the characters until a newline is found.
 * The newline character(s) is consumed at the end.
 */
static void skipLineComment(FILE *fp, int *lineNum)
{
    int ch = fgetc(fp);
    while(ch != '\n')
        ch = fgetc(fp);
    (*lineNum)++;
    skipWhitespace(fp, lineNum);
}

/*
 * Reads in and ignores all characters until the end string is found.
 * The end string is consumed at the end.
 */
static void skipBlockComment(FILE *fp, int *lineNum)
{
    int ch = fgetc(fp);
    while(ch != EOF)
    {
        if (ch == '*')
        {
            ch = fgetc(fp);
            if (ch == '/')
                break;
            else if (ch == '\n')
                (*lineNum)++;
        }
        else if (ch == '\n')
            (*lineNum)++;
        ch = fgetc(fp);
    }
    skipWhitespace(fp, lineNum);
}
