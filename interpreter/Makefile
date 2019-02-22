OPTS = -std=c99 -Wall -Wextra
FLAGS = -c -g
EXECS = interpreter
OBJS = interpreter.o environment.o parser.o lexer.o lexeme.o scanner.o types.o stringBuffer.o

interpreter : $(OBJS)
	gcc $(OBJS) -o interpreter -lm

test : interpreter
	@interpreter test.sea || true

error1 :
	cat ./tests/error1.sea
error1x : interpreter
	@interpreter ./tests/error1.sea || true
error2 :
	cat ./tests/error2.sea
error2x : interpreter
	@interpreter ./tests/error2.sea || true
error3 :
	cat ./tests/error3.sea
error3x : interpreter
	@interpreter ./tests/error3.sea || true
error4 :
	cat ./tests/error4.sea
error4x : interpreter
	@interpreter ./tests/error4.sea || true
error5 :
	cat ./tests/error5.sea
error5x : interpreter
	@interpreter ./tests/error5.sea || true

arrays :
	cat ./tests/arrays.sea
arraysx : interpreter
	interpreter ./tests/arrays.sea

conditionals :
	cat ./tests/conditionals.sea
conditionalsx : interpreter
	interpreter ./tests/conditionals.sea

recursion :
	cat ./tests/recursion.sea
recursionx : interpreter
	interpreter ./tests/recursion.sea

iteration :
	cat ./tests/iteration.sea
iterationx : interpreter
	interpreter ./tests/iteration.sea

functions :
	cat ./tests/functions.sea
functionsx : interpreter
	interpreter ./tests/functions.sea

lambda :
	cat ./tests/lambda.sea
lambdax : interpreter
	interpreter ./tests/lambda.sea

objects :
	cat ./tests/objects.sea
objectsx : interpreter
	interpreter ./tests/objects.sea

problem :
	cat ./tests/problem.sea
problemx : interpreter
	interpreter ./tests/problem.sea

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
