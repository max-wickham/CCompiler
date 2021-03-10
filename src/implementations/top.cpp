#include "top.hpp"
#include "node.hpp"
#include "function.hpp"
#include "statement.hpp"
#include "bindings.hpp"
#include <vector>
#include <iostream>

Top::Top(){}

void Top::printASM(){
    //Bindings *bindings = new Bindings();
    //for(Function * function: functions){
    //    std::cout << "hello" << std::endl;
    //}
    //delete bindings;
    std::cout << "hello" << std::endl;
}

void Top::addFunction(Function *function){
    functions.push_back(function);
}