#include "enum.hpp"
#include <string>
#include "bindings.hpp"
#include "decleration.hpp"
#include "type.hpp"
#include "expression.hpp"
#include "statement.hpp"

EnumEntry::EnumEntry(std::string *id, EnumEntry *enumEntryNext){
    this->id = *id;
    this->enumEntryNext = enumEntryNext;
    this->hasValue = false;
}

EnumEntry::EnumEntry(std::string *id, int value, EnumEntry *enumEntryNext){
    this->id = *id;
    this->value = value;
    this->enumEntryNext = enumEntryNext;
    this->hasValue = true;
}

void EnumEntry::addVariable(Bindings *bindings, int count){
    if(!hasValue){
        this->value = count;
    }
    Int *intT = new Int();
    NumberConstant *constant = new NumberConstant(value);
    Decleration *decleration = new Decleration(intT,&id,constant);
    VariableDefinition *definition = new VariableDefinition(decleration,nullptr);
    definition->printASM(bindings);
    //bindings->addVariable(decleration);
    if(enumEntryNext != nullptr){
        enumEntryNext->addVariable(bindings, ++count);
    }
}

Enum::Enum(std::string *id, EnumEntry *enumEntry){
    this->id = *id;
    this->enumEntry = enumEntry;
}

void Enum::addVariables(Bindings *bindings){
    enumEntry->addVariable(bindings,0);
}