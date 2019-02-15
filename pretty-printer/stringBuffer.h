#ifndef __STRING_BUFFER_INCLUDED__
#define __STRING_BUFFER_INCLUDED__

typedef struct stringBuffer STRING_BUFFER;

extern STRING_BUFFER *newSTRINGBUFFER();
extern void addCharBUFFER(STRING_BUFFER *buffer, int ch);
extern int getLengthBUFFER(STRING_BUFFER *buffer);
extern char *returnStringBUFFER(STRING_BUFFER *buffer);

#endif
