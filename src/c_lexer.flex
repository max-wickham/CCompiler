%option noyywrap

%{

#include       

%}
                                    /*A useful tool : https://regexr.com/ , defining useful regex for later use */
IDENTIFIER [_a-zA-Z][_a-zA-Z0-9]*  
                                    /* see identifer condistions in c https://www.studytonight.com/c/keywords-and-identifier.php#:~:text=Rules%20for%20an%20Identifier%3A,also%20case%20sensitive%20in%20C.&text=Keywords%20are%20not%20allowed%20to%20be%20used%20as%20Identifiers. */
ASSIGNMENT_OPERATOR (([<>][<>]|[*\/%+\-&^|])[=])
                                    /*assigning stuff like += -= etc*/
INTEGERSUFFIX ([uU][lL]|[lL][uU]|[uUlL])
                                    /* defines the suffixes which indicates the type of a literal , u or U = unsigned , l or L = long . etc : https://en.cppreference.com/w/cpp/language/integer_literal */
DECIMALCONSTANT ([1-9][0-9]*)
OCTCONSTANT     ([0][0-7]*)
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
(auto)      {return T_auto;}
(register)  {return T_register;}
(static)    {return T_static;}
(extern)      {return T_extern;}
(typedef)   { return T_typedef; }

   /* Type qualifiers */
(const)     {return T_const;}
(volatile) {return T_volatile;}

 /*control flow statements */
(continue)	{ return T_continue; }
(break)		{ return T_break; }
(goto)		{ return T_goto; }
(case)		{ return T_case; }
(default)	{ return T_default; }
(switch)	   { return T_switch; }

   /* Here should be string and char constants */




   /*Here should be the operators*/



       
   /* added statements */
if		{ return T_if; }
else		{ return T_else; }
return		{ return T_return; }
while		{ return T_while; }
do		{ return T_do; }
for		{ return T_for; }
sizeof		{ return T_sizeof; }

{IDENTIFIER}	                        { yylval.string = new std::string(yytext); return T_identifier; }

(({HEXCONSTANT}|{OCTALCONSTANT})|({DECIMALCONSTANT})){INTEGERSUFFIX}?     { yylval.number = strtol(yytext, NULL, 0); return T_int_const; }

{WHITESPACE}		{ ; }

.                   { fprintf(stderr, "Invalid token\n"); exit(1); }

%%


void yyerror(char const *s) {
     fprintf (stderr, "Parse error : %s\n", s);
     exit(1);
}