OPTS = -std=c99 -Wall -Wextra
FLAGS = -c -g
EXECS = pp
OBJS = pp.o parser.o lexer.o lexeme.o scanner.o types.o stringBuffer.o
TESTFILES = *.1 *.2

pp : $(OBJS)
	gcc $(OBJS) -o pp

run : pp
	@echo Running test 1...
	make test1
	@echo
	@echo Running test 2...
	make test2
	@echo
	@echo Running test 3...
	make test3

test1 : pp ./testfiles/test1.sea
	@echo Original file:
	@cat ./testfiles/test1.sea
	@pp ./testfiles/test1.sea > test1.pp.1
	@echo Pretty printed version of test1:
	@cat test1.pp.1
	@pp test1.pp.1 > test1.pp.2
	@echo Pretty printed version of pretty printed test1:
	@cat test1.pp.2
	diff -s -q test1.pp.1 test1.pp.2
	

test2 : pp ./testfiles/test2.sea
	@echo Original file:
	@cat ./testfiles/test2.sea
	@pp ./testfiles/test2.sea > test2.pp.1
	@echo Pretty printed version of test2:
	@cat test2.pp.1
	@pp test2.pp.1 > test2.pp.2
	@echo Pretty printed version of pretty printed test2:
	@cat test2.pp.2
	diff -s -q test2.pp.1 test2.pp.2

test3 : pp ./testfiles/test3.sea
	@echo Original file:
	@cat ./testfiles/test3.sea
	@pp ./testfiles/test3.sea > test3.pp.1
	@echo Pretty printed version of test3:
	@cat test3.pp.1
	@pp test3.pp.1 > test3.pp.2
	@echo Pretty printed version of pretty printed test3:
	@cat test3.pp.2
	diff -s -q test3.pp.1 test3.pp.2

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
	rm -f *.o $(EXECS) $(TESTFILES)
