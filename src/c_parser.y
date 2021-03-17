/* starting from scratch */
%define parse.error verbose
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
  Decleration *decleration;
  ParameterDefinition *parameterDefinition;
  Parameter *parameter;
  Type *type;
}

%token 
 /* Precendents is very important here!*/
 /*https://www.lysator.liu.se/c/ANSI-C-grammar-y.html USEFUL ! */

      T_identifier T_sc T_comma T_lrb T_lcb T_rcb T_lsb T_rsb T_qm T_colon T_logical_or
			T_logical_and T_or T_xor T_and T_logical_equality T_logical_inequality T_greaterthanequal_op T_lessthanequal_op T_greaterthan_op T_lessthan_op T_shift T_mult T_div
			T_rem T_tilde T_not T_dot T_arrow T_inc T_dec T_add T_sub T_assignment_op T_equal
			T_sizeof T_int_const T_if T_while T_do T_for T_return		
			T_void T_char T_short T_int T_long T_float T_double T_signed T_unsigned
			T_typedef T_static 
			T_volatile T_goto T_break T_continue
			T_case T_default T_switch T_ellipsis T_stringliteral
			
%nonassoc		T_rrb
%nonassoc		T_else
			

%type <top> TOP

%type <statement> STATEMENT SELECTIONSTATEMENT ITERATIONSTATEMENT EXPRESSIONSTATEMENT RETURNSTATEMENT

%type <type>     TYPE

%type<parameterDefinition> PARAMETER

%type <expression> EXPRESSION CONSTANT PRIMARYEXP ARGUMENT POSTFIXEXP UNARYEXP MULTEXP ADDEXP 
                   SHIFTEXP EQUALITYEXP RELATIONALEXP ANDEXP  EXCLUSIVEOREXP 
                   INCLUSIVEOREXP LOGICALANDEXP LOGICALOREXP CONDEXP ASSIGNEXPRESSION  

%type	<string>	T_identifier ASSIGN_OP T_assignment_op T_equal T_and T_add T_sub T_tilde T_not
			          T_mult T_div T_rem T_logical_equality T_logical_inequality T_greaterthanequal_op 
                T_lessthanequal_op T_greaterthan_op T_lessthan_op T_shift T_inc T_dec T_stringliteral  
                T_rsb T_lsb T_rrb T_lrb T_sizeof T_logical_and  T_logical_or T_qm T_colon T_sc T_long  
                T_typedef T_double  T_short T_void T_const

                  
%type <function> FUNCTION


%type	<number>   T_int_const


%start ROOT
                        
%%


ROOT : TOP {g_root = $1; }
     ;

TOP  : FUNCTION {$$ = new Top(); $$->addFunction($1);}
     | TOP FUNCTION{$$->addFunction($2);}    
     ;
    
FUNCTION : TYPE T_identifier PARAMETER STATEMENT {$$ = new Function(new Decleration($1,$2),$4,$3);}
         ;

PARAMETER     : TYPE T_identifier T_rrb             {$$ = new ParameterDefinition(new Decleration($1,$2),nullptr) ; std::cout << "";}
              | TYPE T_identifier T_comma PARAMETER {$$ = new ParameterDefinition(new Decleration($1,$2),$4); std::cout << "";}
              | T_lrb PARAMETER                     {$$ = $2;}
              | T_lrb T_rrb                         {$$ = nullptr;}
              ;

TYPE     : T_void            {$$ = new Void();}
         | T_int             {$$ = new Int(); std::cout << "";}
         | T_char            {$$ = new Char();}
         | T_short           {$$ = new Int();}
         | T_long            {$$ = new Int();}
         | T_float           {$$ = new Float();}
         | T_double          {$$ = new Float();}
         | T_typedef         {$$ = new Int();}
         | T_long TYPE       {$$ = new Int();}
         | T_unsigned TYPE   {$$ = $2; $2->setUnSigned();}
         | T_signed TYPE     {$$ = $2;}
         ;

STATEMENT : ITERATIONSTATEMENT  {$$ = $1;}
          | RETURNSTATEMENT     {$$ = $1;}
          | SELECTIONSTATEMENT  {$$ = $1;}
          | EXPRESSIONSTATEMENT {$$ = $1;}
          | T_rcb               {$$ = nullptr;}
          | T_lcb STATEMENT     {$$ = $2;}
          ;

RETURNSTATEMENT     : T_return EXPRESSION  T_sc STATEMENT {$$ = new ReturnStatement($2,$4); std::cout << "";}

EXPRESSIONSTATEMENT : EXPRESSION T_sc STATEMENT {$$ = new ExpressionStatement($1,$3);}
                    ;

SELECTIONSTATEMENT  : T_if T_lrb EXPRESSION T_rrb STATEMENT STATEMENT                   {$$ = new IfElseStatement($3,$5,nullptr,$6);}            
                    | T_if T_lrb EXPRESSION T_rrb STATEMENT T_else STATEMENT STATEMENT  {$$ = new IfElseStatement($3,$5,$7,$8);}
                    ;

ITERATIONSTATEMENT  : T_while T_lrb EXPRESSION T_rrb STATEMENT STATEMENT {$$ = new WhileLoopStatement($3,$5,$6) ;}
                    | T_do STATEMENT T_while T_lrb EXPRESSION T_rrb T_sc STATEMENT {$$ = new DoWhileLoopStatement($5,$2,$8);}
                    | T_for T_lrb EXPRESSION T_sc EXPRESSION T_sc EXPRESSION T_rrb STATEMENT STATEMENT {$$ = new ForLoopStatement($3,$5,$7,$9,$10);}
                    ;

EXPRESSION : ASSIGNEXPRESSION {$$ = $1;}
           ;


ASSIGNEXPRESSION : CONDEXP {$$ = $1;}
           | UNARYEXP ASSIGN_OP ASSIGNEXPRESSION {  
             if(*$2 == "="){
               $$ = new AssignmentOperator($1,$3);std::cout << "";
             }
             else if(*$2 == "+="){
               {$$ = new AssignmentOperator($1,new AdditionOperator($1,$3));}
             }
             else if(*$2 == "-="){
               {$$ = new AssignmentOperator($1,new SubtractionOperator($1,$3));}
             }
             else if(*$2 == "/="){
               {$$ = new AssignmentOperator($1,new DivisionOperator($1,$3));}
             }
             else if(*$2 == "%="){
               {$$ = new AssignmentOperator($1,new ModuloOperator($1,$3));}
             } 
             else if(*$2 == "&="){
               {$$ = new AssignmentOperator($1,new BitwiseAndOperator($1,$3));}
             }
             else if(*$2 == "^="){
               {$$ = new AssignmentOperator($1,new BitwiseXorOperator($1,$3));}
             }
             else if(*$2 == "|="){
               {$$ = new AssignmentOperator($1,new BitwiseOrOperator($1,$3));}
             }
             else if(*$2 == "<<="){
               {$$ = new AssignmentOperator($1,new ShiftLeftOperator($1,$3));}
             }
             else if(*$2 == ">>="){
               {$$ = new AssignmentOperator($1,new ShiftRightOperator($1,$3));}
             }
             else{
               
             }
           }

ASSIGN_OP   :	T_assignment_op { ; }
	          |	T_equal { ; }
	          ;

CONDEXP    : LOGICALOREXP {$$ = $1;}
           | LOGICALOREXP T_qm EXPRESSION T_colon CONDEXP {$$ = new TernaryOperator($1,$3,$5);}
           ;

  /* here we add the precdents of ops */

LOGICALOREXP  : LOGICALANDEXP {$$ = $1 ;}
              | LOGICALOREXP T_logical_or LOGICALANDEXP {$$ = new LogicalAndOperator($1,$3) ;}
              ;

LOGICALANDEXP : INCLUSIVEOREXP {$$ = $1 ;}
              | LOGICALOREXP T_logical_and INCLUSIVEOREXP  { $$ = new LogicalAndOperator($1,$3) ;}
              ;

INCLUSIVEOREXP : EXCLUSIVEOREXP {$$ = $1 ;}
               | INCLUSIVEOREXP T_or EXCLUSIVEOREXP { $$ = new BitwiseOrOperator($1,$3) ;}
               ;

EXCLUSIVEOREXP : ANDEXP {$$ = $1 ;}
               | EXCLUSIVEOREXP T_xor ANDEXP { $$ = new BitwiseXorOperator($1,$3) ;}
               ;

ANDEXP         : EQUALITYEXP {$$ = $1 ;}
               | ANDEXP T_and EQUALITYEXP { $$ = new LogicalAndOperator($1,$3) ;}
               ;

EQUALITYEXP    : RELATIONALEXP {$$ = $1 ;}
               | EQUALITYEXP T_logical_equality RELATIONALEXP { $$ = new EqualityOperator($1,$3) ;}
               ;

RELATIONALEXP  : SHIFTEXP {$$ = $1 ;}
               | RELATIONALEXP T_lessthan_op SHIFTEXP         {$$ = new LessThanOperator($1,$3);}
               | RELATIONALEXP T_lessthanequal_op SHIFTEXP    {$$ = new LessThanEqualOperator($1,$3);}
               | RELATIONALEXP T_greaterthan_op SHIFTEXP      {$$ = new NotOperator(new LessThanEqualOperator($1,$3));}
               | RELATIONALEXP T_greaterthanequal_op SHIFTEXP {$$ = new NotOperator(new LessThanOperator($1,$3));}
               ;

SHIFTEXP       : ADDEXP {$$ = $1 ;}
               | SHIFTEXP T_shift ADDEXP {} /////////////////////////
               ;
                
ADDEXP         : MULTEXP {$$ = $1 ; std::cout << "";}
               | ADDEXP T_add ADDEXP{ $$ = new AdditionOperator($1,$3) ; std::cout << "";}
               | ADDEXP T_sub ADDEXP{ $$ = new SubtractionOperator($1,$3) ; std::cout << "";}
               ;

MULTEXP        : UNARYEXP {$$ = $1;}
               | MULTEXP T_mult MULTEXP { $$ = new MultiplicationOperator($1,$3);  std::cout << "";}
               | MULTEXP T_div  MULTEXP { $$ = new DivisionOperator($1,$3); }
               | MULTEXP T_rem  MULTEXP { $$ = new ModuloOperator($1,$3); } 
               ; 

UNARYEXP       : POSTFIXEXP {$$ = $1;}
               | T_inc UNARYEXP   {$$ = new AssignmentOperator($2,new AdditionOperator($2,new NumberConstant(1)));}   
               | T_dec UNARYEXP   {$$ = new AssignmentOperator($2,new SubtractionOperator($2,new NumberConstant(1)));} 
               | T_sizeof UNARYEXP {$$ = $2;}        /////////////////////////////////////
               | T_sizeof T_lrb EXPRESSION T_rrb {$$ = new SizeOfOperator($3);} ///////////////////////////////////
               | T_sub UNARYEXP   { $$ = new MultiplicationOperator($2,new NumberConstant(-1)); }
               | T_not UNARYEXP   { $$ = new NotOperator($2); }   
               | T_and UNARYEXP   { $$ = new AddressOperator($2); }   
               ;

POSTFIXEXP     : PRIMARYEXP { $$ = $1; }
               | POSTFIXEXP T_lsb EXPRESSION T_rsb {} ////////////////////////
               | POSTFIXEXP T_dot T_identifier { ;}     ////////////////////////
               | POSTFIXEXP T_arrow T_identifier {}        ////////////////////////
               | POSTFIXEXP T_inc {$$ = new AssignmentOperator($1,new AdditionOperator($1,new NumberConstant(1)));}               ////////////////////////
               | POSTFIXEXP T_dec {$$ = new AssignmentOperator($1,new SubtractionOperator($1,new NumberConstant(1)));}               ////////////////////////
               ;

ARGUMENT       : EXPRESSION T_rrb {$$ = new Parameter($1,nullptr);}
               | EXPRESSION ARGUMENT T_rrb {$$ = new Parameter($1,$2);}
               ;

PRIMARYEXP     : T_identifier           {$$ = new Variable($1); std::cout << "";}
               | CONSTANT               {$$ = $1;}
               | T_stringliteral        {$$ = new StringConstant($1);}
               | T_lrb EXPRESSION T_rrb {$$ = $2;}
               ;

CONSTANT       : T_int_const {$$ = new NumberConstant($1);}
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