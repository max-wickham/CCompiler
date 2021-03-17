#include "decleration.hpp"
#include "node.hpp"
class Type;
class Expression;
//#include "bindings.hpp"
#include <string>

Decleration::Decleration(Type *type, std::string *id){
    this->type = type;
    this->id = *id;
}

Decleration::Decleration(Type *type, std::string *id, Expression *expression){
    this->type = type;
    this->id = *id;
    this->expression = expression;
}