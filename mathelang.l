%{
#include <stdio.h>

%}

digit    [0-9]
letter   [a-zA-Z]
operator [+\-*/=]

%%
{operator} { 
  printf("operator= %s\n", yytext); 
  return 0;
}
{digit}+ { 
  printf("natural number= %s\n", yytext); 
  return 0; 
}
%%

int yywrap(void){return 1;}

int main()
{
  do { yylex(); } while (!feof(stdin));
  return 0;
}


