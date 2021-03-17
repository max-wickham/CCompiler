#include "decleration.hpp"
#include "node.hpp"
class Type;
//#include "expression.hpp"
//#include "bindings.hpp"
#include <string>

Decleration::Decleration(Type *type, std::string *id){
    this->type = type;
    this->id = *id;
}