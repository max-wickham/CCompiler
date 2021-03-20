#ifndef BINDINGS_HPP
#define BINDINGS_HPP

#include <map>
#include <vector>
#include <string>
#include <list>

#include "decleration.hpp"

class Type;

struct BindingData{
    Type *type;
    int offset;
    int size;
};

class Bindings{
    protected:
    int label_count;
    std::list<std::string> break_label;
    std::list<std::string> continue_label;
    std::list<std::string> case_label;
    int current_offset;
    std::list<std::map<std::string, BindingData>> globalBindings;
    std::map<std::string, Type*> *functions;
    std::list<std::map<std::string, BindingData>> bindings;
    public:
    Bindings();
    Bindings(std::map<std::string, Type*> *functions);
    Bindings* createGlobalBindings();
    void addScope();
    void deleteScope();
    void setOffset(int offset);
    int currentOffset();
    int stackPosition(std::string id);
    void addVariable(Decleration *decleration);
    void addFunction(std::string id, Type *type);
    Type* getFunction(std::string id);
    Type* getVariable(std::string id);
    void setBreak(std::string label);
    void setContinue(std::string label);   
    void setCase(std::string label);   
    std::string getBreak();
    std::string getContinue();
    std::string getCase();
    void removeBreak();
    void removeContinue();
    void removeCase();
    std::string createLabel(std::string id);
    Bindings& operator=(const Bindings& rhs);
};

#endif