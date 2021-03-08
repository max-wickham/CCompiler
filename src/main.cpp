#include "c.hpp"

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iomanip>

// Define the instance of the variable that is declared in the histogram.hpp header
TokenValue yylval;

int main()
{
    int sum = 0;
    while(1){
        TokenType type=(TokenType)yylex();

        if(type==NONE){
            break; // No more tokens

        }else if(type==NUMBER){
            // We have a number. The value will be
            // in yylval.numberValue

            // TODO: add to sum
            sum += yylval.numberValue;
            std::cout << "number" << std::endl;
        }else if(type==PLUS){
            std::cout << "plus" << std::endl;
        }else if(type==IF){
            std::cout << "IF" << std::endl;
        }else if(type==STRING){
            std::cout << "STRING " << *yylval.wordValue << std::endl;
        }else{
            assert(0); // There are only three token types.
            return 1;
        }
    }
    std::cout << sum << std::endl;
    return 0;
}
