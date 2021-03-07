#ifndef BINDINGS_HPP
#define BINDINGS_HPP

#include <map>
#include <vector>
#include <string>

#include "type.hpp"

struct BindingData{
    Type *type;
    int offset;
};

class Bindings{
    protected:
    std::string break_label;
    std::string continue_label;
    int begin_offset;
    int current_offset;
    std::map<std::string, BindingData> globalBindings;
    std::map<std::string, BindingData> bindings;
    public:
    Bindings(int begin_offset);
    int currentOffset();
    int stackPosition(std::string id);
    void addVariable(std::string id, Type *type);
    void addFunction(std::string id, Type *type);
    void addLabel(std::string id, int position);
    void setBreak(std::string label);
    void setContinue(std::string label);
    Bindings* createNewBindingsScope();
    //shoudl copy in both global varaibles and the current offset
    std::string getBreak();
    std::string getContinue();
};

#endif