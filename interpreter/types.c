#include "types.h"

/*
 * Author:  Zach Wassynger
 * Date:    1/20/2019
 * Purpose: Part of the designer programming language project of CS 403.
 *          Simply a files of constants for ease of comparison.
 *          This allows for the use of == between these strings.
 */

// Interpreter
char *CLOSURE = "closure";
char *OCLOSURE = "object closure";
char *RETURNED = "returned";
char *BUILT_IN = "built in function";
char *FILE_POINTER = "file";
char *EVAL_EXPR = "eval expr";
char *ARRAY = "array";
char *NULL_VALUE = "null value";

// Environments
char *ENV = "environment";
char *TABLE = "table";
char *IDENTIFIER = "identifier";
char *VALUE = "value";

// Parse trees
char *PROG = "program";
char *MAIN_FUNCTION = "main function";
char *CLASS_DEF = "class definition";
char *CLASS_HEADER = "class header";
char *CLASS_STATEMENTS = "class statements";
char *CLASS_STATEMENT = "class statement";
char *FUNCTION_STATEMENT = "function statement";
char *FUNCTION_INFO = "function info";
char *VAR_DECL = "variable declaration";
char *VAR_LIST = "variable list";
char *VAR_DEF = "variable definition";
//char *EXPR = "expression";
char *EXPR_OP = "expression operation";
char *ID_EXPR = "id expression";
char *UNARY_DEF = "unary def";
char *UNARY_ID = "unary id";
char *UNARY_OP = "unary operation";
char *UNARY_PAREN = "unary parentheses";
char *NEW_OBJECT = "new object";
char *FUNCTION_CALL = "function call";
char *ARRAY_LOOKUP = "array lookup";
char *EXPR_LIST = "expr list";
char *BLOCK = "block";
char *STATEMENTS = "statements";
char *IF_STATEMENT = "if statement";
char *IF_BODY = "if body";
char *ELSE_STATEMENT = "else statement";
char *ELSE_IF_STATEMENT = "else if statement";
//char *DEFINE_STATEMENT = "define statement";
//char *DEFINE_HEADER = "define header";
char *LAMBDA_STATEMENT = "lambda statement";
char *WHILE_STATEMENT = "while statement";
char *DO_WHILE_STATEMENT = "do-while statement";
char *RETURN_STATEMENT = "return statement";
char *BREAK_STATEMENT = "break statement";
char *CONTINUE_STATEMENT = "continue statement";

// Data primatives
char *INTEGER = "integer";
char *REAL = "real";
char *STRING = "string";

// Reserved
char *MAIN = "main";
char *VAR = "var";
char *FUNCTION = "function";
//char *DEFINE = "define";
char *CLASS = "class";
char *EXTENDS = "extends";
char *PUBLIC = "public";
char *PRIVATE = "private";
char *PROTECTED = "protected";
char *NULL_WORD = "null";
char *THIS = "this";
char *NEW = "new";
char *IF = "if";
char *ELSE = "else";
char *WHILE = "while";
char *DO = "do";
char *LAMBDA = "lambda";
char *RETURN = "return";
char *BREAK = "break";
char *CONTINUE = "continue";

// Punctuation
char *SEMICOLON = "semicolon";
char *COMMA = "comma";
char *OBRACE = "open brace";
char *CBRACE = "closed brace";
char *OPAREN = "open parenthesis";
char *CPAREN = "closed parenthesis";
char *OBRACKET = "open bracket";
char *CBRACKET = "closed bracket";

// Operators
char *EQUALS = "equals";
char *PLUS = "plus";
char *MINUS = "dash";
char *TIMES = "asterix";
char *DIVIDE = "back slash";
char *MODULUS = "percent";
char *EXPONENT = "carat";
char *DOT = "dot";
char *LESS_THAN = "less than";
char *GREATER_THAN = "greater than";
char *LESS_THAN_EQUAL = "less than or equal";
char *GREATER_THAN_EQUAL = "greater than or equal";
char *NOTEQUALS = "not equals";
char *EQUALSEQUALS = "double equals";
char *LOGICAL_AND = "double ampersand";
char *LOGICAL_OR = "double vertical bar";
char *BINARY_AND = "ampersand";
char *BINARY_OR = "vertical bar";
// Unary Operators
char *NOT = "not";
char *UMINUS = "unary minus";
char *PLUSPLUS = "double plus";
char *MINUSMINUS = "double minus";

// Other
char *ID = "identifier";
char *END_OF_FILE = "EOF";
char *PARSE_ERROR = "parsing error";
char *BAD_NUM = "incorrect number expression";
