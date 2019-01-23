EXECS = scanner
OBJS = lexer.o types.o lexeme.o scanner.o stringBuffer.o
OPTS = -Wall -Wextra -std=c99
FLAGS = -c

scanner : $(OBJS)
	gcc $(OBJS) -o scanner

lexer.o : lexer.c types.h lexeme.h scanner.h stringBuffer.h
	gcc $(OPTS) $(FLAGS) lexer.c

types.o : types.c types.h
	gcc $(OPTS) $(FLAGS) types.c

lexeme.o : lexeme.c lexeme.h
	gcc $(OPTS) $(FLAGS) lexeme.c

scanner.o : scanner.c scanner.h
	gcc $(OPTS) $(FLAGS) scanner.c

stringBuffer.o : stringBuffer.c stringBuffer.h
	gcc $(OPTS) $(FLAGS) stringBuffer.c

clean :
	rm -f *.o $(EXECS)
