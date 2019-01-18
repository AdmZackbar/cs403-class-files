EXECS =
OPTS = -Wall -Wextra -std=c99
FLAGS = -c

all : types.o lexeme.o scanner.o

types.o : types.c types.h
	gcc $(OPTS) $(FLAGS) types.c

lexeme.o : lexeme.c lexeme.h
	gcc $(OPTS) $(FLAGS) lexeme.c

scanner.o : scanner.c scanner.h
	gcc $(OPTS) $(FLAGS) scanner.c

clean :
	rm -f *.o $(EXECS)
