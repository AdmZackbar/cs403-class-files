EXECS = scanner stringBufferTest
OBJS = lexer.o types.o lexeme.o scanner.o stringBuffer.o
OPTS = -Wall -Wextra -std=c99
FLAGS = -c

scanner : $(OBJS)
	gcc $(OBJS) -o scanner

test1 : scanner
	./scanner ./testfiles/program1.sea

test2 : scanner
	./scanner ./testfiles/program2.sea

test3 : scanner
	./scanner ./testfiles/program3.sea

test4 : scanner
	./scanner ./testfiles/program4.sea

test5 : scanner
	./scanner ./testfiles/program5.sea

stringBufferTest : stringBufferTest.c stringBuffer.c
	gcc stringBufferTest.c stringBuffer.c -o stringBufferTest

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
