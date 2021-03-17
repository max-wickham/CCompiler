#ifndef DECLERATION_HPP
#define DECLERATION_HPP

#include "node.hpp"
class Type;
class Expression;
#include <string>

class Decleration: public Node{
    public:
    Decleration(Type *type, std::string *id);
    Decleration(Type *type, std::string *id, Expression *expression);
    Type *type;
    std::string id;
    Expression *expression;
};

#endif