#ifndef ENUM_HPP
#define ENUM_HPP

#include <string>
#include "bindings.hpp"

class EnumEntry{
protected:
    std::string id;
    int value;
    EnumEntry *enumEntryNext;
    bool hasValue;
public:
    EnumEntry(std::string *id, EnumEntry *enumEntryNext);
    EnumEntry(std::string *id, int value, EnumEntry *enumEntryNext);
    void addVariable(Bindings *bindings, int count);
};

class Enum{
protected:
    std::string id;
    EnumEntry *enumEntry;
public:
    Enum(std::string *id, EnumEntry *enumEntry);
    void addVariables(Bindings *bindings);
};

#endif