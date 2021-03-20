%option noyywrap

%{

#include "c_parser.tab.hpp"  
#include <iostream>   

%}
                                    /*A useful tool : https://regexr.com/ , defining useful regex for later use */
IDENTIFIER [_a-zA-Z][_a-zA-Z0-9]*  
                                    /* see identifer condistions in c https://www.studytonight.com/c/keywords-and-identifier.php#:~:text=Rules%20for%20an%20Identifier%3A,also%20case%20sensitive%20in%20C.&text=Keywords%20are%20not%20allowed%20to%20be%20used%20as%20Identifiers. */
ASSIGNMENT_OPERATOR (([<>][<>]|[*\/%+\-&^|])[=])
                                    /*assigning stuff like += -= etc*/
INTEGERSUFFIX ([uU][lL]|[lL][uU]|[uUlL])
                                    /* defines the suffixes which indicates the type of a literal , u or U = unsigned , l or L = long . etc : https://en.cppreference.com/w/cpp/language/integer_literal */
DECIMALCONSTANT ([1-9][0-9]*)
FLOATCONSTANT ([1-9]*[0-9].[0-9]*)
OCTALCONSTANT     ([0][0-7]*)
HEXCONSTANT     ([0][xX][0-9A-Fa-f]+)
                                    /* Numerical constants simples.  */
CHARCONSTANT (['](([\\]['])|([^']))*['])                                    
                                    /* must be contained within '' , then can be any valid character  , given exceptions for inclusion of other ' i.e '\n'  */
STRINGLITERAL (["](([\\]["])|([^"]))*["])
                                    /* any string like this " adhah " */
WHITESPACE [ \t\r\n]+                 
                                    /* A more comprehensive white space eater that takes carriage returns also opposed to [ \t\n]+  */                                                                                                       
ALL .
                                    /* Catches what's left. */ 
%%
   /*KEYWORDS*/

   /* All the types: */ 
(int)		{ return T_int; }
(void)   {return T_void;}
(char)   {return T_char;}
(short)   {return T_short;}
(long)		{ return T_long; }
(float)		{ return T_float; }
(double)	{ return T_double; }
(signed)	{ return T_signed; }
(unsigned)	{ return T_unsigned; }

   /* storage classes */ 
   /*(auto)      {return T_auto;}*/

(typedef)   { return T_typedef; }

   /* Type qualifiers */
 /*(const)     {return T_const;}*/
 /*control flow statements */
(continue)	{ return T_continue; }
(break)		{ return T_break; }
(goto)		{ return T_goto; }
(default)	{  std::cout << ""; return T_default;}
(switch)	   { return T_switch; }
(case)		{ return T_case; }


   /* Here should be string and char constants */

{STRINGLITERAL}	{ std::string tmp(yytext); yylval.string = new std::string(tmp.substr(1, yyleng-2));
		  return T_stringliteral; }

{CHARCONSTANT} { yylval.number = yytext[1]; return T_int_const; }


   /*Here should be the operators*/

[.][.][.]	{ return T_ellipsis; }
[;]		{ return T_sc; }
[,]		{ return T_comma; }
[(]		{return T_lrb; }
[)]		{return T_rrb; }
[{]             { return T_lcb; }
[}]             { return T_rcb; }
[[]		{ return T_lsb; }
[]]		{ return T_rsb; }
[=]		{ yylval.string = new std::string(yytext); return T_equal; }
{ASSIGNMENT_OPERATOR} { yylval.string = new std::string(yytext); std::cout << ""; return T_assignment_op; }
[?]		{ return T_qm; }
[:]		{ std::cout << ""; return T_colon; }
[\|][\|]		{ return T_logical_or; }
[&][&]		{ return T_logical_and; }
[\|]		{ return T_or; }
[\^]		{ return T_xor; }
[&]		{ yylval.string = new std::string(yytext); return T_and; }
[=][=]		{ yylval.string = new std::string(yytext); return T_logical_equality; }
[!][=]		{ yylval.string = new std::string(yytext); return T_logical_inequality; }
[>][=]    { yylval.string = new std::string(yytext); return T_greaterthanequal_op; }
[<][=]    { yylval.string = new std::string(yytext); return T_lessthanequal_op; }
[>]     { yylval.string = new std::string(yytext); return T_greaterthan_op; }
[<]     { yylval.string = new std::string(yytext); return T_lessthan_op; }
[<|>][<|>]	{ yylval.string = new std::string(yytext); return T_shift; }
[*] 		{ yylval.string = new std::string(yytext); return T_mult; }
[\/]		{ yylval.string = new std::string(yytext); return T_div; }
[%]		{ yylval.string = new std::string(yytext); return T_rem; }
[~]		{ yylval.string = new std::string(yytext); return T_tilde; }
[!]		{ yylval.string = new std::string(yytext); return T_not; }
[.]		{ return T_dot; }
[-][>]		{ return T_arrow; }
[+][+]	{ yylval.string = new std::string(yytext);std::cout << ""; return T_inc; }
[-][-]	{ yylval.string = new std::string(yytext); return T_dec; } 
[+]		{ yylval.string = new std::string(yytext); std::cout << ""; return T_add;}
[-]		{ yylval.string = new std::string(yytext); return T_sub; }


       
   /* added statements */
scope { return T_scope; }
if		{ return T_if; }
else		{ return T_else; }
return		{ return T_return; }
while		{ return T_while; }
do		{ return T_do; }
for		{ return T_for; }
sizeof		{ return T_sizeof; }

{IDENTIFIER}	                        { yylval.string = new std::string(yytext); std::cout << ""; return T_identifier;}

(({HEXCONSTANT}|{OCTALCONSTANT})|({DECIMALCONSTANT})){INTEGERSUFFIX}?     { yylval.number = strtol(yytext, NULL, 0); return T_int_const; }
({FLOATCONSTANT}f)                                                        { yylval.string = new std::string(yytext); return T_float_const;}
({FLOATCONSTANT}d)                                                        { yylval.string = new std::string(yytext); return T_double_constant;}

{WHITESPACE}		{ ; }

.                   { fprintf(stderr, "Invalid token\n"); exit(1); }

%%


void yyerror(char const *s) {
     fprintf (stderr, "Parse error : %s\n", s);
     exit(1);
}