OPTS = -std=c99 -Wall -Wextra
FLAGS = -c -g
EXECS = pp
OBJS = pp.o parser.o lexer.o lexeme.o scanner.o types.o stringBuffer.o

pp : $(OBJS)
	gcc $(OBJS) -o pp

run : recognizer
	@echo Running test 1...
	make test1
	@echo Running test 2...
	make test2
	@echo Running test 3...
	make test3

test1 : recognizer ./testfiles/test1.sea
	./recognizer ./testfiles/test1.sea

test2 : recognizer ./testfiles/test2.sea
	./recognizer ./testfiles/test2.sea

test3 : recognizer ./testfiles/test3.sea
	./recognizer ./testfiles/test3.sea

pp.o : pp.c pp.h parser.h
	gcc $(OPTS) $(FLAGS) pp.c

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
