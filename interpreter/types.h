#ifndef __TYPES_INCLUDED__
#define __TYPES_INCLUDED__

// Interpreter
extern char *CLOSURE;
extern char *OCLOSURE;
extern char *RETURNED;
extern char *BUILT_IN;
extern char *FILE_POINTER;
extern char *EVAL_EXPR;
extern char *ARRAY;
extern char *NULL_VALUE;

// Environments
extern char *ENV;
extern char *TABLE;
extern char *IDENTIFIER;
extern char *VALUE;

// Parse tree
extern char *PROG;
extern char *MAIN_FUNCTION;
extern char *CLASS_DEF;
extern char *CLASS_HEADER;
extern char *CLASS_STATEMENTS;
extern char *CLASS_STATEMENT;
extern char *FUNCTION_STATEMENT;
extern char *FUNCTION_INFO;
extern char *VAR_DECL;
extern char *VAR_LIST;
extern char *VAR_DEF;
//extern char *EXPR;
extern char *EXPR_OP;
extern char *ID_EXPR;
extern char *UNARY_DEF;
extern char *UNARY_ID;
extern char *UNARY_OP;
extern char *UNARY_PAREN;
extern char *NEW_OBJECT;
extern char *FUNCTION_CALL;
extern char *ARRAY_LOOKUP;
extern char *EXPR_LIST;
extern char *BLOCK;
extern char *STATEMENTS;
extern char *IF_STATEMENT;
extern char *IF_BODY;
extern char *ELSE_STATEMENT;
extern char *ELSE_IF_STATEMENT;
//extern char *DEFINE_STATEMENT;
//extern char *DEFINE_HEADER;
extern char *LAMBDA_STATEMENT;
extern char *WHILE_STATEMENT;
extern char *DO_WHILE_STATEMENT;
extern char *RETURN_STATEMENT;
extern char *BREAK_STATEMENT;
extern char *CONTINUE_STATEMENT;

// Data primatives
extern char *INTEGER;
extern char *REAL;
extern char *STRING;

// Reserved
extern char *MAIN;
extern char *VAR;
extern char *FUNCTION;
//extern char *DEFINE;
extern char *CLASS;
extern char *EXTENDS;
extern char *PUBLIC;
extern char *PRIVATE;
extern char *PROTECTED;
extern char *NULL_WORD;
extern char *THIS;
extern char *NEW;
extern char *IF;
extern char *ELSE;
extern char *WHILE;
extern char *DO;
extern char *LAMBDA;
extern char *RETURN;
extern char *BREAK;
extern char *CONTINUE;

// Punctuation
extern char *SEMICOLON;
extern char *COMMA;
extern char *OBRACE;
extern char *CBRACE;
extern char *OPAREN;
extern char *CPAREN;
extern char *OBRACKET;
extern char *CBRACKET;

// Operators
extern char *EQUALS;
extern char *PLUS;
extern char *MINUS;
extern char *TIMES;
extern char *DIVIDE;
extern char *MODULUS;
extern char *EXPONENT;
extern char *DOT;
extern char *LESS_THAN;
extern char *GREATER_THAN;
extern char *LESS_THAN_EQUAL;
extern char *GREATER_THAN_EQUAL;
extern char *NOTEQUALS;
extern char *EQUALSEQUALS;
extern char *LOGICAL_AND;
extern char *LOGICAL_OR;
extern char *BINARY_AND;
extern char *BINARY_OR;
// Unary Operators
extern char *NOT;
extern char *UMINUS;
extern char *PLUSPLUS;
extern char *MINUSMINUS;

// Other
extern char *ID;
extern char *END_OF_FILE;
extern char *PARSE_ERROR;
extern char *BAD_NUM;

#endif
