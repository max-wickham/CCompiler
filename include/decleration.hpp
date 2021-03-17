#ifndef DECLERATION_HPP
#define DECLERATION_HPP

#include "node.hpp"
class Type;
#include <string>

class Decleration: public Node{
    public:
    Decleration(Type *type, std::string *id);
    Type *type;
    std::string id;
};

#endif