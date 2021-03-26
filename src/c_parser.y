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
#include "enum.hpp"
#include "struct.hpp"
#include <iostream>
#include <typeinfo>

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
  Enum *enumm;
  EnumEntry *enumEntry;
  StructDefinition *structDef;
  StructEntry *structEntry;
}

%token 
 /* Precendents is very important here!*/
 /*https://www.lysator.liu.se/c/ANSI-C-grammar-y.html USEFUL ! */

      T_identifier T_sc T_comma T_lrb T_lcb T_rcb T_lsb T_rsb T_qm T_colon T_logical_or
			T_logical_and T_or T_xor T_and T_logical_equality T_logical_inequality T_greaterthanequal_op T_lessthanequal_op T_greaterthan_op T_lessthan_op  T_mult T_div
			T_rem T_tilde T_not T_dot T_arrow T_inc T_dec T_add T_sub T_assignment_op T_equal
			T_sizeof T_int_const T_scope T_if T_while T_do T_for T_return		
			T_void T_char T_short T_int T_long T_float T_double T_signed T_unsigned
			T_typedef T_static T_shift_right T_shift_left
			T_volatile T_goto T_break T_continue
			T_case T_default T_switch T_ellipsis T_stringliteral T_float_const T_double_constant T_enum T_struct
			
%nonassoc		T_rrb
%nonassoc		T_else
			

%type <top> TOP

%type <statement> STATEMENT SELECTIONSTATEMENT ITERATIONSTATEMENT EXPRESSIONSTATEMENT CASESTATEMENT SWITCHSTATEMENT
                  RETURNSTATEMENT VARIABLEDEFINITIONSTATEMENT GLOBALVARIABLE BREAKSTATEMENT CONTINUESTATEMENT
                  STRUCTDECLERATIONSTATEMENT

%type <type>     TYPE

%type<parameterDefinition> PARAMETER

%type <expression> EXPRESSION CONSTANT PRIMARYEXP POSTFIXEXP UNARYEXP MULTEXP ADDEXP 
                   SHIFTEXP EQUALITYEXP RELATIONALEXP ANDEXP  EXCLUSIVEOREXP  INEQUALITYEXPRESSION
                   INCLUSIVEOREXP LOGICALANDEXP LOGICALOREXP CONDEXP ASSIGNEXPRESSION DEREFERENCEEXPRESSION

%type <parameter> ARGUMENT

%type	<string>	T_identifier ASSIGN_OP T_assignment_op T_equal T_and T_add T_sub T_tilde T_not
			          T_mult T_div T_rem T_logical_equality T_logical_inequality T_greaterthanequal_op 
                T_lessthanequal_op T_greaterthan_op T_lessthan_op T_inc T_dec T_stringliteral  
                T_rsb T_lsb T_rrb T_lrb T_sizeof T_logical_and  T_logical_or T_qm T_colon T_sc T_long  
                T_typedef T_double  T_short T_void T_const T_float_const T_double_constant

                  
%type <function> FUNCTION FUNCTIONDEFINITION


%type	<number>   T_int_const

%type <enumm> ENUM

%type <enumEntry> ENUMENTRY

%type <structDef> STRUCTDEFINITION STRUCTDEFINITIONFINAL

%start ROOT
                        
%%


ROOT : TOP {g_root = $1; }
     ;

TOP  : FUNCTION {$$ = new Top(); $$->addFunction($1);}
     | FUNCTIONDEFINITION {$$ = new Top(); $$->addFunctionDefinition($1);}
     | GLOBALVARIABLE {$$ = new Top(); $$->addGlobal((VariableDefinition*)$1);}
     | ENUM           {$$ = new Top(); $$->addEnum($1);}
     | STRUCTDEFINITIONFINAL {$$ = new Top(); $$->addStruct($1); std::cout << "";}
     | TOP FUNCTION {$$ = $1; $$->addFunction($2);}    
     | TOP FUNCTIONDEFINITION {$$ = $1; $$->addFunctionDefinition($2);}    
     | TOP GLOBALVARIABLE {$$ = $1; $$->addGlobal((VariableDefinition*)$2);}
     | TOP ENUM           {$$ = $1; $$->addEnum($2);}
     | TOP STRUCTDEFINITIONFINAL {$$ = $1; $$->addStruct($2); std::cout << "";}
     ;

GLOBALVARIABLE : TYPE T_identifier T_sc  {$$ = new VariableDefinition(new Decleration($1,$2),nullptr);}
               | TYPE T_identifier T_equal EXPRESSION T_sc {$$ = new VariableDefinition(new Decleration($1,$2,$4),nullptr);std::cout << "";}
               | TYPE T_identifier T_lsb T_int_const T_rsb T_sc {$$ = new VariableDefinition(new Decleration(new Array($1,$4),$2),nullptr);}      
               ;


ENUM           : T_enum T_identifier T_lcb ENUMENTRY T_sc {$$ = new Enum($2,$4);std::cout << "";}
               ;

ENUMENTRY      : T_identifier T_rcb   { $$ = new EnumEntry($1, nullptr);std::cout << "";}
               | T_identifier T_equal T_int_const T_rcb { $$ = new EnumEntry($1,$3, nullptr);}
               | T_identifier T_comma ENUMENTRY { $$ = new EnumEntry($1, $3);}
               | T_identifier T_equal T_int_const T_comma ENUMENTRY { $$ = new EnumEntry($1,$3, $5);}
               ;



STRUCTDEFINITIONFINAL : STRUCTDEFINITION T_rcb T_sc {$$ = $1; std::cout << "";}

STRUCTDEFINITION    : T_struct T_identifier T_lcb {$$ = new StructDefinition($2); std::cout << "";}
                    | STRUCTDEFINITION TYPE T_identifier T_sc {$$ = $1; $1->elements.push_back(new StructEntry($2,$3));}
                    ;

    
FUNCTION : TYPE T_identifier PARAMETER STATEMENT {$$ = new Function(new Decleration($1,$2),$4,$3);}
         ;

FUNCTIONDEFINITION : TYPE T_identifier PARAMETER T_sc {$$ = new Function(new Decleration($1,$2),nullptr,nullptr);}

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
         | T_double          {$$ = new Double();}
         | T_typedef         {$$ = new Int();}
         | T_long TYPE       {$$ = new Int();}
         | T_unsigned TYPE   {$$ = $2; $2->setUnSigned();}
         | T_unsigned        {Int* x = new Int(); x->setUnSigned(); $$ = x;}
         | T_signed TYPE     {$$ = $2;}
         | TYPE T_mult       {$$ = new Pointer($1); std::cout << "";}
         ;

STATEMENT : ITERATIONSTATEMENT  {$$ = $1;}
          | RETURNSTATEMENT     {$$ = $1;}
          | SELECTIONSTATEMENT  {$$ = $1;}
          | EXPRESSIONSTATEMENT {$$ = $1; std::cout << "";}
          | VARIABLEDEFINITIONSTATEMENT {$$ = $1; std::cout << "";}
          | T_rcb               {$$ = nullptr; std::cout << "";}
          | T_lcb STATEMENT     {$$ = $2;}
          | BREAKSTATEMENT      {$$ = $1;}
          | CONTINUESTATEMENT   {$$ = $1;}
          | SWITCHSTATEMENT     {$$ = $1;}
          | CASESTATEMENT       {$$ = $1;}
          | STRUCTDECLERATIONSTATEMENT {$$ = $1;}
          ;

BREAKSTATEMENT      : T_break T_sc STATEMENT {$$ = new BreakStatement($3);}

CONTINUESTATEMENT   : T_continue T_sc STATEMENT {$$ = new ContinueStatement($3);}

RETURNSTATEMENT     : T_return EXPRESSION  T_sc STATEMENT {$$ = new ReturnStatement($2,$4); std::cout << "";}

EXPRESSIONSTATEMENT : EXPRESSION T_sc STATEMENT {$$ = new ExpressionStatement($1,$3); std::cout << "";}
                    ;

SELECTIONSTATEMENT  : T_if T_lrb EXPRESSION T_rrb STATEMENT STATEMENT                   {$$ = new IfElseStatement($3,$5,nullptr,$6);}            
                    | T_if T_lrb EXPRESSION T_rrb STATEMENT T_else STATEMENT STATEMENT  {$$ = new IfElseStatement($3,$5,$7,$8);}
                    | T_scope STATEMENT STATEMENT                                       {$$ = new ScopedStatement($2,$3);}
                    ;

ITERATIONSTATEMENT  : T_while T_lrb EXPRESSION T_rrb STATEMENT STATEMENT {$$ = new WhileLoopStatement($3,$5,$6) ;}
                    | T_do STATEMENT T_while T_lrb EXPRESSION T_rrb T_sc STATEMENT {$$ = new DoWhileLoopStatement($5,$2,$8);}
                    | T_for T_lrb T_sc EXPRESSION T_sc EXPRESSION T_rrb STATEMENT STATEMENT {$$ = new ForLoopStatement($4,$6,$8,$9);}
                    | T_for T_lrb TYPE T_identifier T_sc EXPRESSION T_sc EXPRESSION T_rrb STATEMENT STATEMENT {$$ = new ForLoopStatement(new Decleration($3,$4),$6,$8,$10,$11);}
                    | T_for T_lrb EXPRESSION T_sc EXPRESSION T_sc EXPRESSION T_rrb STATEMENT STATEMENT {$$ = new ForLoopStatement($3,$5,$7,$9,$10);}
                    | T_for T_lrb TYPE T_identifier ASSIGN_OP EXPRESSION T_sc EXPRESSION T_sc EXPRESSION T_rrb STATEMENT STATEMENT {$$ = new ForLoopStatement(new Decleration($3,$4,$6),$8,$10,$12,$13);}
                    ;
VARIABLEDEFINITIONSTATEMENT : TYPE T_identifier T_sc STATEMENT                      {$$ = new VariableDefinition(new Decleration($1,$2),$4);}
                            | TYPE T_identifier T_equal EXPRESSION T_sc STATEMENT {$$ = new VariableDefinition(new Decleration($1,$2,$4),$6);std::cout << "";}
                            | TYPE T_identifier T_lsb T_int_const T_rsb T_sc STATEMENT {$$ = new VariableDefinition(new Decleration(new Array($1,$4),$2),$7);}      
                            ;

STRUCTDECLERATIONSTATEMENT  : T_struct T_identifier T_identifier T_sc STATEMENT {$$ = new StructDecleration($2,$3,$5); std::cout << "";}
                            ;

SWITCHSTATEMENT             : T_switch T_lrb EXPRESSION T_rrb T_lcb CASESTATEMENT STATEMENT { $$ = new SwitchStatement($3,$6,$7);}
                            ;


CASESTATEMENT               : T_default T_colon STATEMENT  {$$ = new CaseStatement(nullptr,$3,nullptr); std::cout << "" <<std::endl;}
                            | T_case CONSTANT T_colon STATEMENT CASESTATEMENT {$$ = new CaseStatement($2,$4,$5); }
                            | T_case CONSTANT T_colon STATEMENT {$$ = new CaseStatement($2,$4,nullptr); }
                            ;

EXPRESSION : ASSIGNEXPRESSION {$$ = $1;}
           ;

ASSIGNEXPRESSION : CONDEXP {$$ = $1;}
           | UNARYEXP ASSIGN_OP ASSIGNEXPRESSION {  
             if(*$2 == "="){
               $$ = new AssignmentOperator($1,$3); std::cout <<  "";
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

ASSIGN_OP   :	T_assignment_op {$$ = $1 ; }
	          |	T_equal { $$ = $1 ; std::cout <<  "";}
	          ;

CONDEXP    : LOGICALOREXP {$$ = $1;}
           | LOGICALOREXP T_qm EXPRESSION T_colon CONDEXP {$$ = new TernaryOperator($1,$3,$5);}
           ;

  /* here we add the precdents of ops */

LOGICALOREXP  : LOGICALANDEXP {$$ = $1 ;}
              | LOGICALOREXP T_logical_or LOGICALANDEXP {$$ = new LogicalOrOperator($1,$3) ;}
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
               | ANDEXP T_and EQUALITYEXP { $$ = new BitwiseAndOperator($1,$3) ;}
               ;

EQUALITYEXP    : INEQUALITYEXPRESSION {$$ = $1 ;}
               | EQUALITYEXP T_logical_equality RELATIONALEXP { $$ = new EqualityOperator($1,$3) ;}
               ;

INEQUALITYEXPRESSION    : RELATIONALEXP {$$ = $1 ;}
                        | INEQUALITYEXPRESSION T_logical_inequality INEQUALITYEXPRESSION { $$ = new NotOperator(new EqualityOperator($1,$3));}
                        ;

RELATIONALEXP  : SHIFTEXP {$$ = $1 ;}
               | RELATIONALEXP T_lessthan_op SHIFTEXP         {$$ = new LessThanOperator($1,$3);}
               | RELATIONALEXP T_lessthanequal_op SHIFTEXP    {$$ = new LessThanEqualOperator($1,$3);}
               | RELATIONALEXP T_greaterthan_op SHIFTEXP      {$$ = new NotOperator(new LessThanEqualOperator($1,$3));}
               | RELATIONALEXP T_greaterthanequal_op SHIFTEXP {$$ = new NotOperator(new LessThanOperator($1,$3));}
               ;

SHIFTEXP       : ADDEXP {$$ = $1 ;}
               | SHIFTEXP T_shift_right SHIFTEXP {$$ = new ShiftRightOperator($1,$3);} /////////////////////////
               | SHIFTEXP T_shift_left SHIFTEXP {$$ = new ShiftLeftOperator($1,$3);} 
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

UNARYEXP       : DEREFERENCEEXPRESSION {$$ = $1;}
               | T_identifier T_dot T_identifier { $$ = new DotOperator($1,$3);}
               | T_inc UNARYEXP   {$$ = new AssignmentOperator($2,new AdditionOperator($2,new NumberConstant(1)));}   
               | T_dec UNARYEXP   {$$ = new AssignmentOperator($2,new SubtractionOperator($2,new NumberConstant(1)));} 
               | T_sizeof UNARYEXP {$$ = $2;}        /////////////////////////////////////
               | T_sizeof T_lrb TYPE T_rrb {$$ = new SizeOfOperator($3);} ///////////////////////////////////
               | T_sizeof T_lrb EXPRESSION T_rrb {$$ = new SizeOfExpressionOperator($3);} ///////////////////////////////////
               | T_sizeof TYPE {$$ = new SizeOfOperator($2);} ///////////////////////////////////
               | T_sizeof EXPRESSION {$$ = new SizeOfExpressionOperator($2);} ///////////////////////////////////
               | T_sub UNARYEXP   { $$ = new MultiplicationOperator($2,new NumberConstant(-1)); }
               | T_not UNARYEXP   { $$ = new NotOperator($2); }   
               | T_and UNARYEXP   { $$ = new AddressOperator($2); std::cout << "";}
               | T_identifier T_lrb ARGUMENT { $$ = new FunctionCall($1,$3);}
               | T_identifier T_lrb T_rrb { $$ = new FunctionCall($1,nullptr);}


               ;

DEREFERENCEEXPRESSION : POSTFIXEXP {$$ = $1;}
                      | T_mult PRIMARYEXP {$$ = new DefreferenceOperator($2);  std::cout << "";}
                      ;

POSTFIXEXP     : PRIMARYEXP { $$ = $1; }
               | POSTFIXEXP T_lsb EXPRESSION T_rsb {$$ = new IndexOperator($1,$3);} ////////////////////////
               | POSTFIXEXP T_inc {$$ = new AssignmentOperator($1,new AdditionOperator($1,new NumberConstant(1)));}               ////////////////////////
               | POSTFIXEXP T_dec {$$ = new AssignmentOperator($1,new SubtractionOperator($1,new NumberConstant(1)));}               ////////////////////////
               ;

ARGUMENT       : EXPRESSION T_rrb {$$ = new Parameter($1,nullptr);}
               | EXPRESSION T_comma ARGUMENT {$$ = new Parameter($1,$3);}
               ;

PRIMARYEXP     : T_identifier           {$$ = new Variable($1);}
               | CONSTANT               {$$ = $1;std::cout << "";}
               | T_stringliteral        {$$ = new StringConstant($1);}
               | T_lrb EXPRESSION T_rrb {$$ = $2;}
               ;

CONSTANT       : T_int_const {$$ = new NumberConstant($1);}
               | T_float_const {$$ = new FloatConstant(std::stof($1->substr(0,$1->length())));}
               | T_double_constant {$$ = new DoubleConstant(std::stof($1->substr(0,$1->length())));}
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