%{
#define YY_DECL extern int yylex()
#include <stdio.h>
#include <string.h>

extern int yylineno;
extern int yylex();


#include "test.tab.h"
%}

%option never-interactive
