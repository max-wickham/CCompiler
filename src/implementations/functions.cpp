#include "node.hpp"
#include "type.hpp"
#include "statement.hpp"
#include "parameter.hpp"
#include "bindings.hpp"
#include "function.hpp"
#include <string>
#include <iostream>

Function::Function(Type *type, std::string& id, Statement* statement,
        ParameterDefinition *firstParameter = nullptr){
            this->type = type;
            this->id = id;
            this->statement = statement;
            this->firstParameter = firstParameter;
}

void Function::printASM(Bindings *bindings){
    //add the function to the current scope
    std::string label = bindings->createLabel("function");
    std::cout << label << ":";
    bindings->addFunction(id,label);
    //load the variables into the bindings, 
    //each parameter should set its current stack position in the bindings
    firstParameter->createScope(bindings);
    //then print the asm of the statement
    statement->printASM(bindings);
}