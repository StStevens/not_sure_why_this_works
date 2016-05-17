%{
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

extern int yylex();
extern FILE *yyin;

void yyerror(const char *s);
%}
