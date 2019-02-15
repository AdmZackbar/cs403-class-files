#include <stdlib.h>
#include <assert.h>
#include "stringBuffer.h"

/*
 * Author:  Zach Wassynger
 * Date:    1/22/2019
 * Purpose: Part of the designer programming language project of CS 403.
 *          Essentially a dynamic array for characters. It starts out with a fixed
 *          size, then grows as needed.
 */

struct stringBuffer
{
    char *store;
    int size, length;
};

STRING_BUFFER *newSTRINGBUFFER()
{
    STRING_BUFFER *buffer = malloc(sizeof(STRING_BUFFER));
    assert(buffer != 0);
    
    buffer->size = 80;
    buffer->length = 0;
    buffer->store = malloc(sizeof(char)*buffer->size);
    assert(buffer->store != 0);
    
    return buffer;
}
/*
 * Adds a character to the buffer, increasing the memory of the buffer as
 * necessary.
 */
void addCharBUFFER(STRING_BUFFER *buffer, int ch)
{
    assert(buffer != 0);
    
    if(buffer->length >= buffer->size)
    {
        buffer->size *= 2;
        buffer->store = realloc(buffer->store, sizeof(char)*buffer->size);
        assert(buffer->store != 0);
    }
    buffer->store[buffer->length++] = ch;
}

int getLengthBUFFER(STRING_BUFFER *buffer)
{
    assert(buffer != 0);

    return buffer->length;
}

/*
 * Cleans up the string in the buffer by reallocating the size, null terminating
 * the string, and freeing up the buffer. As it sounds, the buffer itself is
 * destroyed in the process of this function.
 */
char *returnStringBUFFER(STRING_BUFFER *buffer)
{
    assert(buffer != 0);
    
    char *string = buffer->store;
    
    string = realloc(string, sizeof(char)*(buffer->length+1));
    assert(string != 0);
    string[buffer->length] = '\0';
    
    free(buffer);
    return string;
}
