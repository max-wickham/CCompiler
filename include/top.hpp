#ifndef TOP_HPP
#define TOP_HPP

#include "node.hpp"
#include "function.hpp"
#include "statement.hpp"
#include "bindings.hpp"
#include "struct.hpp"
#include "enum.hpp"
#include <vector>

class Top : public Node{
    protected:
    std::vector<Function*> functions;
    std::vector<Function*> functionDefinitions;
    std::vector<VariableDefinition*> globalVariables;
    std::vector<Enum*> enums;
    std::vector<StructDefinition*> structs;
    public:
    Top();
    void printASM();
    void addFunction(Function *function);
    void addFunctionDefinition(Function *function);
    void addGlobal(VariableDefinition *definition);
    void addEnum(Enum *enumDef);
    void addStruct(StructDefinition* structDefinition);
};

#endif