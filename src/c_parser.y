/* starting from scratch */

%code requires{

#include "/implementations/bindings.cpp"
#include "/implementations/decleration.cpp"
#include "/implementations/expression.cpp"
#include "/implementations/functions.cpp"
#include "/implementations/parameter.cpp"
#include "/implementations/statement.cpp"
#include "/implementations/type.cpp"

 #include <cassert>

extern const Expression *g_root; // A way of getting the AST out ??????

int yylex(void);
void yyerror(const char *);
    

}

// ???????/ need to define AST node avec max
%union{
  const Expression *expr;
  double number;
  std::string *string;
}

%token 
 /* Precendents is very important here!*/

T_identifier T_sc T_lcb T_rcb T_lrb T_rrb
T_return T_int_const 
T_int 
 


%start ROOT
                        
%%


ROOT : TOP { g_root = $1; }
     ;

TOP  :T_int T_identifier T_lrb T_rrb T_lcb STATEMENT T_rcb {$$ = new function($1,$2,$6);}     
     ;

STATEMENT : T_return T_int_const T_sc {$$ = new return($2);}
          ;

%%

const Expression *g_root; // Definition of variable (to match declaration earlier)

const Expression *parseAST()
{
  g_root=0;
  yyparse();
  return g_root;
}