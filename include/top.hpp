#ifndef TOP_HPP
#define TOP_HPP

#include "node.hpp"
#include "function.hpp"
#include "statement.hpp"
#include "bindings.hpp"
#include <vector>

class Top : public Node{
    public:
    std::vector<Function> functions;
    std::vector<VariableDefinition> globalVariables;
    void printASM();
    void addFunction();
};

#endif