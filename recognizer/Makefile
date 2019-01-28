OPTS = -std=c99 -Wall -Wextra
FLAGS = -c -g
EXECS = recognizer
OBJS = recognizer.o lexer.o lexeme.o scanner.o types.o stringBuffer.o

recognizer : $(OBJS)
	gcc $(OBJS) -o recognizer

run : recognizer
	@echo Running test 1...
	make test1
	@echo Running test 2...
	make test2
	@echo Running test 3...
	make test3
	@echo Running test 4...
	make test4
	@echo Running test 5...
	make test5

test1 : recognizer
	./recognizer ./testfiles/program1.sea

test2 : recognizer
	./recognizer ./testfiles/program2.sea

test3 : recognizer
	./recognizer ./testfiles/program3.sea

test4 : recognizer
	./recognizer ./testfiles/program4.sea

test5 : recognizer
	./recognizer ./testfiles/program5.sea

recognizer.o : recognizer.c lexer.h lexeme.h types.h
	gcc $(OPTS) $(FLAGS) recognizer.c

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
