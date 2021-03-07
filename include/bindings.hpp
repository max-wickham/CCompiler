#ifndef BINDINGS_HPP
#define BINDINGS_HPP

#include <map>
#include <vector>
#include <string>

#include "type.hpp"
#include "decleration.hpp"

struct BindingData{
    Type *type;
    int offset;
    int size;
};

class Bindings{
    protected:
    static int label_count;
    std::string break_label;
    std::string continue_label;
    int begin_offset;
    int current_offset;
    std::map<std::string, BindingData> globalBindings;
    std::map<std::string, BindingData> bindings;
    std::map<std::string, std::string> functions;
    public:
    Bindings(int begin_offset);
    int currentOffset();
    int stackPosition(std::string id);
    void addVariable(Decleration *decleration);
    void addFunction(std::string id, std::string label);
    void addLabel(std::string id, int position);
    void setBreak(std::string label);
    void setContinue(std::string label);
    Bindings* createNewBindingsScope();
    //shoudl copy in both global varaibles and the current offset
    std::string getBreak();
    std::string getContinue();
    std::string createLabel(std::string id);
    Bindings& operator=(const Bindings& rhs) {};
};

#endif