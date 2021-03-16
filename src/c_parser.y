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

%type <statement> STATEMENT SELECTIONSTATEMENT ITERATIONSTATEMENT EXPRESSIONSTATEMENT 

%type <type>     TYPE

%type<declaration> PARAMETER

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
     | TYPE T_identifier {$$ = new Top(); $$->addGlobalVariable( new Decleration($1,$2));}
     | TOP TYPE T_identifier{$$->addGlobalVariable(new Decleration($1,$2));}
     ;
    
FUNCTION : TYPE T_identifier PARAMETER STATEMENT {$$ = new Function(new Decleration($1,$2),$3,$4,nullptr);}
         ;

PARAMETER     : TYPE T_identifier T_rrb             {$$ = ParameterDefinition(new Decleration($1,$2),nullptr) ;}
              | TYPE T_identifier T_comma PARAMETER {$$ =  ParameterDefinition(new Decleration($1,$2),$4);}
              | T_lrb PARAMETER                     {$$ = $2;}
              ;

TYPE     : T_void            {$$ = $1  ;}
         | T_int             {$$ = new Int() ;}
         | T_char            {$$ = new Char();}
         | T_short           {$$ = $1 ;}
         | T_long            {$$ = $1 ;}
         | T_float           {$$ = new Float() ;}
         | T_double          {$$ = $1 ;}
         | T_typedef         {$$ = $1 ;}
         | T_long TYPE       {$$ = $1 ;}
         | T_unsigned TYPE   {$$ = $2 ; $2->setUnSigned()}
         | T_signed TYPE     {$$ = $2}
         ;

STATEMENT : ITERATIONSTATEMENT  {$$ = $1;}
          | SELECTIONSTATEMENT  {$$ = $1;}
          | EXPRESSIONSTATEMENT {$$ = $1;}
          | T_rcb               {$$ = nullptr;}
          | T_lcb STATEMENT     {$$ = $2;}
          ;

EXPRESSIONSTATEMENT : EXPRESSION T_sc STATEMENT {$$ = new ExpressionStatement($1,$3);}
                    ;

SELECTIONSTATEMENT  : T_if T_lrb EXPRESSION T_rrb STATEMENT STATEMENT                   {$$ = new IfElseStatement($3,$5,$6);}            
                    | T_if T_lrb EXPRESSION T_rrb STATEMENT T_else STATEMENT STATEMENT  {$$ = new IfElseStatement($5,$7);}
                    | T_switch T_lrb EXPRESSION T_rrb STATEMENT STATEMENT               {$$ = new SwitchStatement($5);}
                    ;

ITERATIONSTATEMENT  : T_while T_lrb EXPRESSION T_rrb STATEMENT STATEMENT {$$ = new WhileLoopStatement($3,$5,$6) ;}
                    | T_do STATEMENT T_while T_lrb EXPRESSION T_rrb T_sc STATEMENT {$$ = new DoWhileLoopStatement($5,$2,$8);}
                    | T_for T_lrb EXPRESSION T_sc EXPRESSION T_sc EXPRESSION T_rrb STATEMENT STATEMENT {$$ = new ForLoopStatement($3,$4,$5,$7,$8);}
                    ;

EXPRESSION : ASSIGNEXPRESSION {$$ = $1}
           ;


ASSIGNEXPRESSION : CONDEXP {$$ = $1;}
           | UNARYEXP ASSIGN_OP ASSIGNEXPRESSION {  
             if(*%2 == "="){
               $$ = new AssignmentOperator($1,$3);
             }
             elseif(*$2 == "+="){
               {$$ = new AssignmentOperator($1,new AdditionOperator($1,$3));}
             }
             elseif(*$2 == "-="){
               {$$ = new AssignmentOperator($1,new SubtractionOperator($1,%3));}
             }
             elseif(*$2 == "/="){
               {$$ = new AssignmentOperator($1,new DivisionOperator($1,%3));}
             }
             elseif(*$2 == "%="){
               {$$ = new AssignmentOperator($1,new ModuloOperator($1,%3));}
             } 
             elseif(*$2 == "&="){
               {$$ = new AssignmentOperator($1,new BitwiseAndOperator($1,%3));}
             }
             elseif(*$2 == "^="){
               {$$ = new AssignmentOperator($1,new BitwiseXOROperator($1,%3));}
             }
             elseif(*$2 == "|="){
               {$$ = new AssignmentOperator($1,new BitwiseOrOperator($1,%3));}
             }
             elseif(*$2 == "<<="){
               {$$ = new AssignmentOperator($1,new ShiftLeftOperator($1,%3));}
             }
             elseif(*$2 == ">>="){
               {$$ = new AssignmentOperator($1,new ShiftRightOperator($1,%3));}
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
               | EXCLUSIVEOREXP T_xor ANDEXP { $$ = new BitwiseXOROperator($1,$3) ;}
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
               | RELATIONALEXP T_greaterthan_op SHIFTEXP      {$$ = new GreaterThanOperator($1,$3);}
               | RELATIONALEXP T_greaterthanequal_op SHIFTEXP {$$ = new GreaterThanOperatorEqual($1,$3);}
               ;

SHIFTEXP       : ADDEXP {$$ = $1 ;}
               | SHIFTEXP T_shift ADDEXP { $$ = new ;} /////////////////////////
               ;
                
ADDEXP         : MULTEXP {$$ = $1 ;}
               | ADDEXP T_add MULTEXP{ $$ = new AdditionOperator($1,$3) ;}
               | ADDEXP T_sub MULTEXP{ $$ = new SubtractionOperator($1,$3) ;}
               ;

MULTEXP        : UNARYEXP
               | MULTEXP T_mult UNARYEXP { $$ = new MultiplicationOperator($1,$3); }
               | MULTEXP T_div  UNARYEXP { $$ = new DivisionOperator($1,$3); }
               | MULTEXP T_rem  UNARYEXP { $$ = new ModuloOperator($1,$3); } 
               ; 

UNARYEXP       : POSTFIXEXP {$$ = $1}
               | T_inc UNARYEXP   {$$ = new AssignmentOperator($1,new AdditionOperator($1,new NumberConstant(1)));}   
               | T_dec UNARYEXP   {$$ = new AssignmentOperator($1,new SubtractionOperator($1,new NumberConstant(1)));} 
               | T_sizeof UNARYEXP {$$ = $2}        /////////////////////////////////////
               | T_sizeof T_lrb TYPE T_rrb {$$ = $1} ///////////////////////////////////
               | T_sub UNARYEXP   { $$ = new MultiplicationOperator($2,new NumberConstant(-1)); }
               | T_not UNARYEXP   { $$ = new NotOperator($2); }   
               | T_and UNARYEXP   { $$ = new AddressOperator($2); }   
               ;

POSTFIXEXP     : PRIMARYEXP { $$ = $1; }
               | POSTFIXEXP T_lsb EXPRESSION T_rsb {$$ = new Array($1,$2);} ////////////////////////
               | POSTFIXEXP T_dot T_identifier {$$ = DotOperator($1) ;}     ////////////////////////
               | POSTFIXEXP T_arrow T_identifier {$$ = ArrowOp($1);}        ////////////////////////
               | POSTFIXEXP T_inc {$$ = new AssignmentOperator($1,new AdditionOperator($1,new NumberConstant(1)));}               ////////////////////////
               | POSTFIXEXP T_dec {$$ = new AssignmentOperator($1,new SubtractionOperator($1,new NumberConstant(1)));}               ////////////////////////
               ;

ARGUMENT       : EXPRESSION T_rrb {$$ = new Parameter($1,nullptr);}
               | EXPRESSION ARGUMENT T_rrb {$$ = new Parameter($1,$2);}
               ;

PRIMARYEXP     : T_identifier {}        {$$ = new Variable($1);}
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