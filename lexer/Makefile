EXECS = scanner
OBJS = lexer.o types.o lexeme.o scanner.o
OPTS = -Wall -Wextra -std=c99
FLAGS = -c

scanner : lexer.o types.o lexeme.o scanner.o
	gcc $(OBJS) -o scanner

lexer.o : lexer.c types.h lexeme.h scanner.h
	gcc $(OPTS) $(FLAGS) lexer.c

types.o : types.c types.h
	gcc $(OPTS) $(FLAGS) types.c

lexeme.o : lexeme.c lexeme.h
	gcc $(OPTS) $(FLAGS) lexeme.c

scanner.o : scanner.c scanner.h
	gcc $(OPTS) $(FLAGS) scanner.c

clean :
	rm -f *.o $(EXECS)
