This is the pretty printer for the SEA programming language.
Like the lexer, it has one command line argument - the name of the file to be formatted.
Each file passed in to the executable 'pp' will print out the formatted version to stdout.

Makefile commands:
    make - compiles and creates the 'pp' executable
    make run - makes 'pp' if applicable and runs 3 test cases(in the testfile directory)
