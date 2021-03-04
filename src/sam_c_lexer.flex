%option noyywrap

%{

#include "sam_c_parser.tab.hpp"         //WTF IS THIS CUNT?

%}

IDENTIFIER [_a-zA-Z][_a-zA-Z0-9]*

INTEGERSUFFIX ([uU][lL]|[lL][uU]|[uUlL])

DECIMALCONSTANT ([1-9][0-9]*)

WHITESPACE [ \t\r\n]+

ALL .

%%
(int)		{ return T_INT; }


[{]             { return T_LCB; }
[}]             { return T_RCB; }
[(]		        { return T_LRB; }
[)]		        { return T_RRB; }
[;]             { return T_SC; }
       

return		{ return T_RETURN; }

{IDENTIFIER}	                        { yylval.string = new std::string(yytext); return T_IDENTIFIER; }
({DECIMALCONSTANT}){INTEGERSUFFIX}?     { yylval.number = strtol(yytext, NULL, 0); return T_INT_CONST; }

{WHITESPACE}		{ ; }

.                   { fprintf(stderr, "Invalid token\n"); exit(1); }

%%


void yyerror(char const *s) {
     fprintf (stderr, "Parse error : %s\n", s);
     exit(1);
}