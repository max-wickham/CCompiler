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


 #include <cassert>

extern const Top *g_root; // A way of getting the AST out ??????
                          // 
int yylex(void);
void yyerror(const char *);
    

}

// ???????/ need to define AST node avec max
%union{
  const Top *top;
  double number;
  std::string *string;
  Statement *statement;
}

%token 
 /* Precendents is very important here!*/
 /*https://www.lysator.liu.se/c/ANSI-C-grammar-y.html USEFUL ! */

      T_identifier T_sc T_comma T_lrb T_lcb T_rcb T_lsb T_rsb T_qm T_colon T_logical_or
			T_logical_and T_or T_xor T_and T_logical_equality T_logical_inequality T_rel_op T_shift T_mult T_div
			T_rem T_tilde T_not T_dot T_arrow T_inc_dec T_addsub_OP T_assignment_op T_equal
			T_sizeof T_int_const T_if T_while T_do T_for T_return		
			T_void T_char T_short T_int T_long T_float T_double T_signed T_unsigned
			T_typedef T_static T_auto T_register
			T_const T_volatile T_goto T_break T_continue
			T_case T_default T_switch T_ellipsis T_stringliteral
			
%nonassoc		T_rrb
%nonassoc		T_else
			

%type <top> TOP
%type <number> T_int_const
%type <string> T_return T_int T_sc T_lcb T_rcb T_lrb T_rrb T_identifier 
%type <Statement> STATEMENT 
%type <Expression> EXPR


%start ROOT
                        
%%


ROOT : TOP { g_root = $1; }
     ;

TOP  :T_int T_identifier T_lrb T_rrb T_lcb STATEMENT T_rcb {$$ = new function($1,$2,$6,nullptr);}     
     ;

STATEMENT : T_return EXPR T_sc {$$ = new Return($2,nullptr);}
          ;

EXPR      : T_int_const {$$ = new NumberConstant($1);}
          ;

 
%%

const Top *g_root; // Definition of variable (to match declaration earlier)

const Top *parseAST()
{
  g_root=0;
  yyparse();
  return g_root;
}