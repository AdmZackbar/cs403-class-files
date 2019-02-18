OPTS = -std=c99 -Wall -Wextra
FLAGS = -c -g
EXECS = interpreter
OBJS = interpreter.o environment.o parser.o lexer.o lexeme.o scanner.o types.o stringBuffer.o

interpreter : $(OBJS)
	gcc $(OBJS) -o interpreter

interpreter.o : interpreter.c parser.h
	gcc $(OPTS) $(FLAGS) interpreter.c

environment.o : environment.c environment.h lexeme.h
	gcc $(OPTS) $(FLAGS) environment.c

parser.o : parser.c parser.h lexer.h lexeme.h types.h
	gcc $(OPTS) $(FLAGS) parser.c

lexer.o : lexer.c lexer.h types.h lexeme.h
	gcc $(OPTS) $(FLAGS) lexer.c

lexeme.o : lexeme.c lexeme.h types.h
	gcc $(OPTS) $(FLAGS) lexeme.c

scanner.o : scanner.c scanner.h
	gcc $(OPTS) $(FLAGS) scanner.c

types.o : types.c types.h
	gcc $(OPTS) $(FLAGS) types.c

stringBuffer.o : stringBuffer.c stringBuffer.h
	gcc $(OPTS) $(FLAGS) stringBuffer.c

clean :
	rm -f *.o $(EXECS)
