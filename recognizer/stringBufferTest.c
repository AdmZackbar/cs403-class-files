#include <stdio.h>
#include "stringBuffer.h"

int main(int argc, char **argv)
{
    STRING_BUFFER *buffer = newSTRINGBUFFER();

    for(int i=65; i<91; i++)
    {
        addCharBUFFER(buffer, (char)i);
    }

    printf("%s\n", returnStringBUFFER(buffer));

    return 0;
}