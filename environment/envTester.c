#include "environment.h"
#include "types.h"

static LEXEME *createEnv();
static void insertEnv(LEXEME *env, char *name, LEXEME *value);
static void findValue(LEXEME *env, char *name);
static LEXEME *addScope(LEXEME *env, LEXEME *vars, LEXEME *vals);
static void printEnv(LEXEME *env);

int main()
{
    LEXEME *env = createEnv();
    insertEnv(env, "x", newLEXEMEint(3, -1));
    printEnv(env);
    LEXEME *vars = cons(IDENTIFIER, newLEXEMEstring(ID, "y", -1), cons(IDENTIFIER, newLEXEMEstring(ID, "z", -1), NULL));
    LEXEME *vals = cons(VALUE, newLEXEMEint(4, -1), cons(VALUE, newLEXEMEstring(STRING, "hello", -1), NULL));
    env = addScope(env, vars, vals);
    printEnv(env);
    findValue(env, "x");
}

static LEXEME *createEnv()
{
    printf("Creating a new environment\n");
    return newEnvironment();
}

static void insertEnv(LEXEME *env, char *name, LEXEME *value)
{
    printf("Adding variable %s with value ", name);
    displayLEXEME(stdout, value);
    printf("\n");
    insertEnvironment(env, newLEXEMEstring(ID, name, -1), value);
}

static void findValue(LEXEME *env, char *name)
{
    printf("Value of %s: ", name);
    LEXEME *value = getValueEnv(env, newLEXEMEstring(ID, name, -1));
    displayLEXEME(stdout, value);
    printf("\n");
}

static LEXEME *addScope(LEXEME *env, LEXEME *vars, LEXEME *vals)
{
    LEXEME *newScope = newScopeEnv(env, vars, vals);
    printf("Extending the environment with ");
    while (vars != NULL)
    {
        displayLEXEME(stdout, car(vars));
        printf(":");
        displayLEXEME(stdout, car(vals));
        if (cdr(vars) != NULL)
            printf(", ");
        vars = cdr(vars);
        vals = cdr(vals);
    }
    printf("\n");
    return newScope;
}

static void printEnv(LEXEME *env)
{
    printf("The environment is:\n");
    displayEnvironment(stdout, env);
}
