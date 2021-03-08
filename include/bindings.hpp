#ifndef BINDINGS_HPP
#define BINDINGS_HPP

#include <map>
#include <vector>
#include <string>
#include <list>

#include "type.hpp"
#include "decleration.hpp"

struct BindingData{
    Type *type;
    int offset;
    int size;
};

class StackObject{
    protected:
    std::string id;
    BindingData bindingData;
    StackObject *nextObject;
    public:

}

class Bindings{
    protected:
    static int label_count;
    std::string break_label;
    std::string continue_label;
    int current_offset;
    std::list<std::map<std::string, BindingData>> globalBindings;
    //this should be shared across all bindings
    static std::map<std::string, Type*> functions;
    std::list<std::map<std::string, BindingData>> bindings;
    public:
    Bindings();
    void addScope();
    void deleteScope();
    void setOffset(int offset);
    int currentOffset();
    int stackPosition(std::string id);
    void addVariable(Decleration *decleration);
    void addFunction(std::string id, Type *type);
    Type* getFunction(std::string id);
    Type* getVariable(std::string id);
    //void addLabel(std::string id, int position);
    void setBreak(std::string label);
    void setContinue(std::string label);   
    std::string getBreak();
    std::string getContinue();
    std::string createLabel(std::string id);
    Bindings& operator=(const Bindings& rhs) {};
};

#endif