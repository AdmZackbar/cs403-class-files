OPTS = -std=c99 -Wall -Wextra
FLAGS = -c -g
EXECS = interpreter
OBJS = interpreter.o environment.o parser.o lexer.o lexeme.o scanner.o types.o stringBuffer.o

interpreter : $(OBJS)
	gcc $(OBJS) -o interpreter -lm

test : interpreter
	@make errors
	@make features
	@make problem
	@make problemx

errors :
	@make error1
	@make error1x || true
	@make error2
	@make error2x || true
	@make error3
	@make error3x || true
	@make error4
	@make error4x || true
	@make error5
	@make error5x || true

features :
	@make arrays
	@make arraysx
	@make conditionals
	@make conditionalsx
	@make recursion
	@make recursionx
	@make iteration
	@make iterationx
	@make functions
	@make functionsx
	@make lambda
	@make lambdax
	@make objects
	@make objectsx

error1 :
	cat tests/error1.sea
error1x : interpreter
	./run tests/error1.sea
error2 :
	cat tests/error2.sea
error2x : interpreter
	./run tests/error2.sea
error3 :
	cat tests/error3.sea
error3x : interpreter
	./run tests/error3.sea
error4 :
	cat tests/error4.sea
error4x : interpreter
	./run tests/error4.sea
error5 :
	cat tests/error5.sea
error5x : interpreter
	./run tests/error5.sea

arrays :
	cat tests/arrays.sea
arraysx : interpreter
	./run ./tests/arrays.sea

conditionals :
	cat tests/conditionals.sea
conditionalsx : interpreter
	./run ./tests/conditionals.sea

recursion :
	cat tests/recursion.sea
recursionx : interpreter
	./run ./tests/recursion.sea

iteration :
	cat tests/iteration.sea
iterationx : interpreter
	./run ./tests/iteration.sea

functions :
	cat tests/functions.sea
functionsx : interpreter
	./run ./tests/functions.sea

lambda :
	cat tests/lambda.sea
lambdax : interpreter
	./run ./tests/lambda.sea

objects :
	cat tests/objects.sea
objectsx : interpreter
	./run ./tests/objects.sea

problem :
	cat iAVL
problemx : interpreter
	./run iAVL commands

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
