%code requires{

#include ""

extern Node* g_root; // A way of getting the AST out
void setTypeInformation(Type* type_ptr, std::string type_str);

//! This is to fix problems when generating C++
// We are declaring the functions provided by Flex, so
// that Bison generated code can call them.
int yylex(void);
void yyerror(const char *);

}

%token			T_IDENTIFIER T_SC T_LRB T_RCB T_LCB
                T_RETURN
                T_INT

%nonassoc T_RRB

%type	<function>	FunctionDefinition

%type	<string>	T_IDENTIFIER

%start ROOT
                        
%%

ROOT:
		TranslationUnit { g_root = $1; }
		;




%%

Node* g_root; // Definition of variable (to match declaration earlier)

Node* parseAST()
{
    g_root = 0;
    yyparse();
    return g_root;
}
