The environments module is designed to hold information about various scopes in a program.

extern LEXEME *newEnvironment()
 - Creates a new, empty environment with an empty table of variables and values
 - Returns the lexeme that represents the environment
 
extern LEXEME *insertEnvironment(LEXEME *env, LEXEME *id, LEXEME *value)
 - Inserts a variable(id)-value(value) pair into the local environment(env)
 - Returns the value that was inserted

extern LEXEME *getValueEnv(LEXEME *env, LEXEME *id)
 - Finds the value associated with the given variable name(id) in the environment(env) given.
 - Searches the local environment first, then iterates through to the global environment.
 - If no value is found, the program is terminated early with a code of -10.
 - Returns the value(if found)

extern LEXEME *setValueEnv(LEXEME *env, LEXEME *id, LEXEME *newVal)
 - Finds the given variable(id) in the environment(env) tables, then sets its value to the given value(newVal)
 - Searches the local environment first, then iterates through to the global environment.
 - If no value is found, the program is terminated early with a code of -11.
 - Returns the old value associated with that variable

extern LEXEME *newScopeEnv(LEXEME *env, LEXEME *vars, LEXEME *vals)
 - Creates a new local environment and puts it on top of the given environment(env)
 - Sets the new environment to have the given table of variables(vars) and values(vals)
 - Returns the new environment

extern void displayEnvironment(FILE *fp, LEXEME *env)
 - Displays the entire list of environments associated with the given one(env) to the given stream(fp)
 - Environments are printed in the order of local -> global

extern void displayLocalEnv(FILE *fp, LEXEME *env)
 - Displays only the most local environment of the one given(env) to the given stream(fp)
