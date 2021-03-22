#include "top.hpp"
#include "node.hpp"
#include "function.hpp"
#include "statement.hpp"
#include "bindings.hpp"
#include <vector>
#include <iostream>
#include "enum.hpp"

Top::Top(){}

void Top::printASM(){
    std::map<std::string, Type*> functionsMapOriginal;
    
    for(Function * function: functions){
        function->addToMap(functionsMapOriginal);
    }

    for(Function * function: functionDefinitions){
        function->addToMap(functionsMapOriginal);
    }
    
    std::map<std::string, Type*> *functionsMap = &functionsMapOriginal;
    Bindings *bindings = new Bindings(functionsMap);

    bindings->addScope();
    for(Function * function: functions){
        Bindings *newBindings = bindings->createGlobalBindings();
        for(StructDefinition* structDef: structs){
            newBindings->addStruct(structDef->id,structDef);
        }
        function->printASM(newBindings,globalVariables,enums);
        delete newBindings;
    }
}

void Top::addFunction(Function *function){
    functions.push_back(function);
}

void Top::addFunctionDefinition(Function *function){
    functionDefinitions.push_back(function);
}


void Top::addGlobal(VariableDefinition *definition){
    globalVariables.push_back(definition);
}

void Top::addEnum(Enum *enumDef){
    enums.push_back(enumDef);
}

void Top::addStruct(StructDefinition* structDefinition){
    structs.push_back(structDefinition);
}