#ifndef histogram_hpp
#define histogram_hpp

#include <string>

enum TokenType{
    NONE,
    YYEOF, // This indicates there are no more tokens
    T_PLUS,
    T_EQUALS,
    T_NUMBER
};

union TokenValue{
    double numberValue;
    std::string *wordValue;
};

// This is a global variable used to move the
// attribute value from the lexer back to the
// main program.
// By convention it is called yylval, as that is
// the name that will be automatically generated
// by Bison (see next lab).
extern TokenValue yylval;

// This is the lexer function defined by flex. Each
// time it is called, a token type value will be
// returned.
extern int yylex();

#endif
