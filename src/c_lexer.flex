%option noyywrap

%{
/* Now in a section of C that will be embedded
   into the auto-generated code. Flex will not
   try to interpret code surrounded by %{ ... %} */

/* Bring in our declarations for token types and
   the yylval variable. */
#include "c.hpp"
#include <vector>
#include <iostream>

// This is to work around an irritating bug in Flex
// https://stackoverflow.com/questions/46213840/get-rid-of-warning-implicit-declaration-of-function-fileno-in-flex
extern "C" int fileno(FILE *stream);
/* End the embedded code section. */
%}


%%
"if"     {return IF;}
"else"   {return ELSE;}
"while"  {return WHILE;}
"int"    {return INT;}
"bool"   {return BOOL;}
"return" {return RETURN;} 

-?([0-9]+\.?[0-9]*)   {yylval.numberValue = std::stod(yytext); return NUMBER;}


[a-zA-Z_]?\"(\\.|[^\\"])*\"  {yylval.wordValue = new std::string(yytext);
                              *yylval.wordValue = (*yylval.wordValue).substr(1,(*yylval.wordValue).size()-2);
                              return STRING;}

"+"      {return PLUS;}
"-"      {return MINUS;}
"*"      {return MULTIPLY;}
"/"      {return DIVIDE;}
","      {return COMMA;}

.     {fprintf(stderr, "%s\n",yytext); }

%%

/* Error handler. This will get called if none of the rules match. */
void yyerror (char const *s)
{
  fprintf (stderr, "Flex Error: %s\n", s); /* s is the text that wasn't matched */
  exit(1);
}
