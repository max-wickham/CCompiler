/* starting from scratch */

%code requires{

#include "/implementations/bindings.hpp"
#include "/implementations/decleration.hpp"
#include "/implementations/expression.hpp"
#include "/implementations/functions.hpp"
#include "/implementations/parameter.hpp"
#include "/implementations/statement.hpp"
#include "/implementations/type.hpp"

 #include <cassert>

extern const Top *g_root; // A way of getting the AST out ??????

int yylex(void);
void yyerror(const char *);
    

}

// ???????/ need to define AST node avec max
%union{
  const Top *top;
  double number;
  std::string *string;
}

%token 
 /* Precendents is very important here!*/

T_identifier T_sc T_lcb T_rcb T_lrb T_rrb
T_return T_int_const 
T_int 

%type <top> TOP
%type <number> T_int_const
%type <string> T_return T_int T_sc T_lcb T_rcb T_lrb T_rrb T_identifier 
 


%start ROOT
                        
%%


ROOT : TOP { g_root = $1; }
     ;

TOP  :T_int T_identifier T_lrb T_rrb T_lcb STATEMENT T_rcb {$$ = new function($1,$2,$6);}     
     ;

STATEMENT : T_return EXPR T_sc {$$ = new Return($2);}
          ;

EXPR      : T_int_const {$$ = new Number_Constant($1);}
          ;

%%

const Top *g_root; // Definition of variable (to match declaration earlier)

const Top *parseAST()
{
  g_root=0;
  yyparse();
  return g_root;
}