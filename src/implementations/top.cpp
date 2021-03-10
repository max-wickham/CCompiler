#include "top.hpp"
#include "node.hpp"
#include "function.hpp"
#include "statement.hpp"
#include "bindings.hpp"
#include <vector>

Top::Top(){}

void Top::printASM(){
    Bindings *bindings = new Bindings();
    for(Function * function: functions){
        function->printASM(bindings);
    }
    delete bindings;
}

void Top::addFunction(Function *function){
    functions.push_back(function);
}