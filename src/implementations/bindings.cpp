#include <map>
#include <vector>
#include <string>

#include "type.hpp"
#include "decleration.hpp"
#include "bindings.hpp"

Bindings::Bindings(int begin_offset){
    this->begin_offset = begin_offset;
}

void Bindings::addVariable(Decleration *decleration){
    //first find the memory size needed
    BindingData bindingData;
    bindingData.type = decleration->type;
    bindingData.offset = current_offset;
    bindingData.size = decleration->type->getSize(decleration->arraySize);
    current_offset = bindingData.offset + bindingData.size;
    bindings.insert(std::pair<std::string,BindingData>(decleration->id,bindingData));
}

std::string Bindings::createLabel(std::string id){
    std::string response = id + "_" + std::to_string(label_count);
    label_count++;
    return response;
}