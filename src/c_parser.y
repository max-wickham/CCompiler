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
 /*https://www.lysator.liu.se/c/ANSI-C-grammar-y.html USEFUL ! */

T_identifier T_comma T_sc T_lcb T_rcb T_lrb
T_return T_int_const 
T_int 

%nonassoc		T_rrb

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

Functiondef     : Declarator T_lrb Parameter CompoundStatement {}
                ;


Parameter       : ParameterDeclarator { }
                ; 

ParameterDeclarator      : Declarator t_rrb {$$ = new }
                         | Declartor T_comma ParameterDeclarator {}
                ;
VariableDeclarator      : Declarator T_sc {$$ = new }
                ;

Declarator      : DeclarationType IDENTIFIER { }
                ;

DeclarationType : T_int      {$$ = new Int(); }
                | T_void     {$$ = new Int(); }
                | T_char     {$$ = new Int(); }
                | T_short    {$$ = new Int(); }
                | T_long     {$$ = new Int(); }
                | T_float    {$$ = new Int(); }
                | T_double   {$$ = new Int(); }
                | T_signed   {$$ = new Int(); }
                | T_unsigned {$$ = new Int(); }
                ;

CompoundStatement : Statement 
                  | T_lcb CompoundStatement T_rcb 
                  |  Statement CompoundStatement 
                  | 
                  |
                  ;

Statement        : If_Statement
                 | 
                 |
                 |



%%

const Top *g_root; // Definition of variable (to match declaration earlier)

const Top *parseAST()
{
  g_root=0;
  yyparse();
  return g_root;
}