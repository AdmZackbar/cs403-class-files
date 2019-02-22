#include <stdlib.h> // For exit()
#include "environment.h"
#include "types.h"

LEXEME *newEnvironment()
{
    return cons(ENV, cons(TABLE, NULL, NULL), NULL);
}

LEXEME *insertEnvironment(LEXEME *env, LEXEME *id, LEXEME *value)
{
    setCar(car(env), cons(ID, id, car(car(env))));
    setCdr(car(env), cons(IDENTIFIER, value, cdr(car(env))));
    return value;
}

LEXEME *getValueEnv(LEXEME *env, LEXEME *id)
{
    LEXEME *vars, *vals;
    
    //printf("Looking for: %s\n", getStrLEXEME(id));
    //displayEnvironment(stdout, env); // TODO REMOVE

    while (env != NULL)
    {
        vars = car(car(env));
        vals = cdr(car(env));
        while (vars != NULL)
        {
            if (sameVar(id, car(vars)))
                return car(vals);
            vars = cdr(vars);
            vals = cdr(vals);
        }
        env = cdr(env);
    }
    fprintf(stderr, "Error: unidentified variable ");
    displayLEXEME(stderr, id);
    fprintf(stderr, "\n");
    exit(-10);
}

LEXEME *setValueEnv(LEXEME *env, LEXEME *id, LEXEME *newVal)
{
    LEXEME *vars, *vals;
    
    while (env != NULL)
    {
        vars = car(car(env));
        vals = cdr(car(env));
        while (vars != NULL)
        {
            if (sameVar(id, car(vars)))
            {
                LEXEME *oldVal = car(vals);
                setCar(vals, newVal);
                return oldVal;
            }
            vars = cdr(vars);
            vals = cdr(vals);
        }
        env = cdr(env);
    }
    fprintf(stderr, "Error: unidentified variable ");
    displayLEXEME(stderr, id);
    fprintf(stderr, "\n");
    exit(-11);
}

LEXEME *newScopeEnv(LEXEME *env, LEXEME *vars, LEXEME *vals)
{
    return cons(ENV, cons(TABLE, vars, vals), env);
}

void displayEnvironment(FILE *fp, LEXEME *env)
{
    LEXEME *var, *val;
    int numEnv = 0;
    while (env != NULL)
    {
        fprintf(fp, "Environment %d:\n", ++numEnv);
        var = car(car(env));
        val = cdr(car(env));
        while (var != NULL)
        {
            displayLEXEME(fp, car(var));
            fprintf(fp, ": ");
            displayLEXEME(fp, car(val));
            fprintf(fp, "(%s)\n", getTypeLEXEME(car(val)));
            var = cdr(var);
            val = cdr(val);
        }
        env = cdr(env);
    }
}

void displayLocalEnv(FILE *fp, LEXEME *env)
{
    LEXEME *var = car(car(env));
    LEXEME *val = cdr(car(env));
    fprintf(fp, "Local Environment:\n");
    while (var != NULL)
    {
        displayLEXEME(fp, car(var));
        fprintf(fp, ": ");
        displayLEXEME(fp, car(val));
        fprintf(fp, "\n");
        var = cdr(var);
        val = cdr(val);
    }
}
