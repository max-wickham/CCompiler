/* starting from scratch */

%code requires{

#include "bindings.hpp"
#include "decleration.hpp"
#include "expression.hpp"
#include "function.hpp"
#include "parameter.hpp"
#include "statement.hpp"
#include "type.hpp"
#include "top.hpp"
#include <iostream>


 #include <cassert>

extern Top *g_root; // A way of getting the AST out ??????
                          // 
int yylex(void);
void yyerror(const char *);
    

}

%union{
  Top *top;
  double number;
  std::string *string;
  Statement *statement;
  Function *function;
  Expression *expression;
  ReturnStatement *returnstatement;
  Int *intt;
}

%token 
 /* Precendents is very important here!*/
 /*https://www.lysator.liu.se/c/ANSI-C-grammar-y.html USEFUL ! */

      T_identifier T_sc T_comma T_lrb T_lcb T_rcb T_lsb T_rsb T_qm T_colon T_logical_or
			T_logical_and T_or T_xor T_and T_logical_equality T_logical_inequality T_rel_op T_shift T_mult T_div
			T_rem T_tilde T_not T_dot T_arrow T_inc T_dec T_add T_sub T_assignment_op T_equal
			T_sizeof T_int_const T_if T_while T_do T_for T_return		
			T_void T_char T_short T_int T_long T_float T_double T_signed T_unsigned
			T_typedef T_static 
			T_volatile T_goto T_break T_continue
			T_case T_default T_switch T_ellipsis T_stringliteral
			
%nonassoc		T_rrb
%nonassoc		T_else
			

%type <top> TOP
%type <number> T_int_const
%type <string> T_return T_sc T_lcb T_rcb T_lrb T_rrb T_identifier 
%type <statement> STATEMENT 
%type <expression> EXPR
%type <function> FUNCTION
%type <intt> INT


%start ROOT
                        
%%


ROOT : TOP {g_root = $1; }
     ;

TOP  : EXTERNAL {$$ = new Top(); $$->addFunction($1);}
     | TOP EXTERNAL{$$->addFunction($2);}     
     ;

EXTERNAL : FUNCTION {$$ = $1;}
         | DECLARATION {$$ = $1} /* global var ?*/
         ;
    
FUNCTION : TYPE DECLARATOR PARAMETER STATEMENT {$$ = new Function($1,$2,$6,nullptr);}
         ;

PARAMETER     : TYPE DECLARATOR T_rrb             {$$ = new ;}
              | TYPE DECLARATOR T_comma PARAMETER {$$ = new ;}
              | T_lrb PARAMETER                   {$$ = $2;}
              ;


DECLARATOR    : T_identifier {$$ = new ;}
              | T_identifier T_lsb T_rsb {$$ = new ;}
              | T_lrb DECLARATOR T_rrb   {$$ = new ;}
              | DECLARATOR T_lrb T_rrb   {$$ = new ;}
              ;

TYPE     : T_void            {$$ = new ;}
         | T_int             {$$ = new ;}
         | T_char            {$$ = new ;}
         | T_short           {$$ = new ;}
         | T_long            {$$ = new ;}
         | T_float           {$$ = new ;}
         | T_double          {$$ = new ;}
         | T_signed          {$$ = new ;}
         | T_unsigned        {$$ = new ;}
         | T_typedef         {$$ = new ;}
         | T_long TYPE       {$$ = new ;}
         | T_unsigned TYPE   {$$ = new ;}
         ;

STATEMENT : COMPOUNDSTATEMENT   {$$ = $1;}
          | ITERATIONSTATEMENT  {$$ = $1;}
          | SELECTIONSTATEMENT  {$$ = $1;}
          | EXPRESSIONSTATEMENT {$$ = $1;}
          | T_rcb               {$$ = nullptr;}
          | T_lcb STATEMENT     {$$ = $2;}
          ;

EXPRESSIONSTATEMENT : T_sc           {$$ = new ;}
                    | EXPRESSIONSTATEMENT T_sc STATEMENT {$$ = new ExpressionStatement($1,$3)}
                    ;

SELECTIONSTATEMENT  : T_if T_lrb EXPRESSION T_rrb STATEMENT STATEMENT                   {$$ = new IfElseStatement($3,$5,$6);}            
                    | T_if T_lrb EXPRESSION T_rrb STATEMENT T_else STATEMENT STATEMENT           {$$ = new IfElseStatement($5,$7);}
                    | T_switch T_lrb EXPRESSION T_rrb STATEMENT STATEMENT  {$$ = new SwitchStatement($5);}
                    ;

ITERATIONSTATEMENT  : T_while T_lrb EXPRESSION T_rrb STATEMENT STATEMENT {$$ = new ;}
                    | T_do STATEMENT T_while T_lrb EXPRESSION T_rrb T_sc STATEMENT {$$ = new ;}
                    | T_for T_lrb EXPRESSION T_sc EXPRESSION T_sc EXPRESSION T_rrb STATEMENT STATEMENT {$$ = new ;}
                    ;

EXPRESSION :
           | UNARYEXP ASSIGN_OP ASSIGNMENT {  
             if(*%2 == "="){
               $$ = new AssignExpression($1,$3);
             }
             if()
           }

ASSIGN_OP   :	T_assignment_op { ; }
	          |	T_eq { ; }
	          ;

CONDEXP    : LOGICALOREXP {$$ = $1;}
           | LOGICALO
           ;

  /* here we add the precdents of ops */

LOGICALOREXP  : LOGICALANDEXP {$$ = $1 ;}
              | LOGICALOREXP T_logical_or LOGICALAND {$$ = new ;}
              ;

LOGICALANDEXP : INCLUSIVEOREXP {$$ = $1 ;}
              | LOGICALOREXP T_logical_and INCLUSIVEOREXP
              ;
INCLUSIVEOREXP : EXCLUSIVEOREXP {$$ = $1 ;}
               | INCLUSIVEOREXP T_or EXCLUSIVEOREXP { $$ = new ;}
               ;

EXCLUSIVEOREXP : ANDEXP {$$ = $1 ;}
               | EXCLUSIVEOREXP T_xor ANDEXP { $$ = new ;}
               ;

ANDEXP         : EQUALITYEXP {$$ = $1 ;}
               | ANDREXP T_and EQUALITYEXP { $$ = new ;}
               ;

EQUALITYEXP    : RELATIONALEXP {$$ = $1 ;}
               | EQUALITYEXP T_logical_equality RELATIONALEXP { $$ = new ;}
               ;

RELATIONALEXP  : SHIFTEXP {$$ = $1 ;}
               | RELATIONALEXP T_rel_op SHIFTEXP { $$ = new ;}
               ;

SHIFTEXP       : ADDEXP {$$ = $1 ;}
               | SHIFTEXP T_shift ADDEXP { $$ = new ;}
               ;
                
ADDEXP         : MULTEXP {$$ = $1 ;}
               | ADDEXP T_addsub_OP MULTEXP{ $$ = new AdditionOperator(ADD) ;}
               | ADDEXP T_addsub_OP MULTEXP{ $$ = new AdditionOperator(ADD) ;}
               ;

MULTEXP        : UNARYEXP
               | MULTEXP T_mult UNARYEXP
               | MULTEXP T_div  UNARYEXP
               | MULTEXP T_rem  UNARYEXP
               ;

UNARYEXP       : POSTFIXEXP {$$ = $1}
               | T_inc_dec UNARYEXP
               | T_sizeof UNARYEXP {$$ = $2}
               | T_sizeof T_lrb TYPE  T_rrb 
               ;

UNARYOPS       : T_and       { $$ = $1; }
               | T_addsub_OP { $$ = $1; }
               | T_mult      { $$ = $1; }
               | T_tilde     { $$ = $1; }
               | T_not       { $$ = $1; }
               ;

POSTFIXEXP     : PRIMARYEXP { $$ = $1; }
               | POSTFIXEXP T_lsb EXPRESSION T_rsb {$$ = new ;}
               | POSTFIXEXP T_lrb POSTFIXEXP2 {$$ = new ;}
               | POSTFIXEXP T_dot T_identifier {$$ = $1 ;}
               | POSTFIXEXP T_arrow T_identifier {$$ = $1;}
               | POSTFIXEXP T_inc {$$ = new IncOperator($1)} 
               | POSTFIXEXP T_dec {$$ = new DecOperator($1) }   
               ;

ARGUMENT       : EXPRESSION T_rrb {$$ = new Parameter($1,nullptr);}
               | EXPRESSION ARGUMENT T_rrb {$$ = new Parameter($1,$2);}
               ;

POSTFIXEXP2    : T_rrb {$$ = $1}
               ;

PRIMARYEXP     : T_identifier {}        {$$ = new Variable($1); }
               | CONSTANT               {$$ = $1;}
               | T_stringliteral        {$$ = new StringConstant($1) ; }
               | T_lrb EXPRESSION T_rrb {$$ = $2;}
               ;

CONSTANT       : T_int_const {$$ = new NumberConstant($1);}
               ;



INT : T_int {$$ = new Int();}
    ; 

 
%%

Top *g_root; // Definition of variable (to match declaration earlier)

Top *parseAST()
{
  //yyin  = fopen(fileIn.c_str(), "r"); 
  g_root=0;
  yyparse();
  return g_root;
}