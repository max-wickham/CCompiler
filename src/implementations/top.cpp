#include "top.hpp"
#include "node.hpp"
#include "function.hpp"
#include "statement.hpp"
#include "bindings.hpp"
#include <vector>
#include <iostream>

Top::Top(){}

void Top::printASM(){
    for(Function * function: functions){
        Bindings *bindings = new Bindings();
        function->printASM(bindings);
        delete bindings;
    }
}

void Top::addFunction(Function *function){
    functions.push_back(function);
}