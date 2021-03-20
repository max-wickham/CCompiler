#ifndef TOP_HPP
#define TOP_HPP

#include "node.hpp"
#include "function.hpp"
#include "statement.hpp"
#include "bindings.hpp"
#include <vector>

class Top : public Node{
    protected:
    std::vector<Function*> functions;
    std::vector<VariableDefinition*> globalVariables;
    public:
    Top();
    void printASM();
    void addFunction(Function *function);
    void addGlobal(VariableDefinition *definition);
};

#endif