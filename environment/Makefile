OBJS = types.o lexeme.o environment.o envTester.o
FLAGS = -c
OPTS = -std=c99 -Wall -Wextra
EXECS = environment

environment : $(OBJS)
	gcc $(OPTS) $(OBJS) -o environment

run : environment
	./environment

envTester.o : envTester.c environment.h
	gcc $(OPTS) $(FLAGS) envTester.c

environment.o : environment.c environment.h lexeme.h
	gcc $(OPTS) $(FLAGS) environment.c

lexeme.o : lexeme.c lexeme.h types.h
	gcc $(OPTS) $(FLAGS) lexeme.c

types.o : types.c types.h
	gcc $(OPTS) $(FLAGS) types.c

clean :
	rm -f *.o $(EXECS)
