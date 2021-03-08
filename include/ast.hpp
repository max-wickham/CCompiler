#ifndef AST_HPP
#define AST_HPP

#include "node.hpp"
#include "function.hpp"
#include "statement.hpp"
#include "bindings.hpp"
#include <vector>

class AST : public Node{
    public:
    std::vector<Function> functions;
    std::vector<VariableDefinition> globalVariables;
    void printASM(Bindings *bindings);
};

#endif