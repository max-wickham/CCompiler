#include "top.hpp"
#include "node.hpp"
#include "function.hpp"
#include "statement.hpp"
#include "bindings.hpp"
#include <vector>
#include <iostream>

Top::Top(){}

void Top::printASM(){
    std::map<std::string, Type*> functionsMapOriginal;
    for(Function * function: functions){
        function->addToMap(functionsMapOriginal);
    }
    std::map<std::string, Type*> *functionsMap = &functionsMapOriginal;
    Bindings *bindings = new Bindings(functionsMap);
    for(VariableDefinition *definition: globalVariables){
        definition->printASM(bindings);
    }
    for(Function * function: functions){
        Bindings *newBindings = bindings->createGlobalBindings();
        function->printASM(newBindings);
        delete newBindings;
    }
}

void Top::addFunction(Function *function){
    functions.push_back(function);
}

void Top::addGlobal(VariableDefinition *definition){
    globalVariables.push_back(definition);
}