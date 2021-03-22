#ifndef STRUCT_HPP
#define STRUCT_HPP

#include <string>
#include <list>
class Type;

// class Struct {
//     public:
//     std::string id;
//     std::map<std::string,Type*> elements;
//     Struct(std::string *id);
//     //void addElement(std::string id, Type *type);
// };
class StructEntry{
    public:
    StructEntry(Type *type, std::string *id){this->id = *id; this->type = type;}
    std::string id;
    Type *type; 
};

class StructDefinition{
    public:
    StructDefinition(std::string *id){this->id = *id;}
    std::string id;
    std::list<StructEntry*> elements;
};

#endif