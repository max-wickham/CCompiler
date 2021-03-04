#ifndef DECLERATION_HPP
#define DECLERATION_HPP

#include "node.hpp"
#include "type.hpp"
#include "expression.hpp"
#include <string>

//int x = 5
class Decleration: public Node{
    protected:
    Type *type;
    std::string id;
    Expression *initialiser;
    public:
    Decleration(Type *type, std::string id, Expression *initialiser = nullptr);
};

//int x,y,x
class DeclerationList: public Node{
    protected:
    Decleration *decleration;
    DeclerationList *declerationListNext;
    public:
    DeclerationList(Decleration *decleration, DeclerationList *declerationListNext = nullptr);
};

#endif