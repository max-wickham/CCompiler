#include <map>
#include <vector>
#include <string>

#include "type.hpp"
#include "decleration.hpp"
#include "bindings.hpp"

Bindings::Bindings(){
    std::map<std::string,BindingData> map;
    bindings.push_front(map);
}

void Bindings::addScope(){
    std::map<std::string,BindingData> map;
    bindings.push_front(map);
}

void Bindings::deleteScope(){
    bindings.pop_front();
}

void Bindings::setOffset(int offset){
    this->current_offset = offset;
}

int Bindings::currentOffset(){
    return current_offset;
}

int Bindings::stackPosition(std::string id){
    std::list<std::map<std::string, BindingData>>::iterator it;
    for (it = bindings.begin(); it != bindings.end(); ++it){
        if(it->find(id) != it->end()){
            return it->at(id).offset;
        }
    }
    return 0;
}

void Bindings::addVariable(Decleration *decleration){
    BindingData data;
    data.type = decleration->type;
    data.size = decleration->type->getSize();
    data.offset = current_offset;
    current_offset -= data.size;
    bindings.begin()->insert(std::pair<std::string,BindingData>(decleration->id,data));
}

void Bindings::addFunction(std::string id, Type *type){
    functions.insert(std::pair<std::string,Type*>(id,type));
}

Type* Bindings::getFunction(std::string id){
    return functions.at(id);
}

Type* Bindings::getVariable(std::string id){
    std::list<std::map<std::string, BindingData>>::iterator it;
    for (it = bindings.begin(); it != bindings.end(); ++it){
        if(it->find(id) != it->end()){
            return it->at(id).type;
        }
    }
    return 0;
}

void Bindings::setBreak(std::string label){
    break_label = label;
}

void Bindings::setContinue(std::string label){
    continue_label = label;
}

std::string Bindings::getBreak(){
    return break_label;
}
std::string Bindings::getContinue(){
    return continue_label;
}

std::string Bindings::createLabel(std::string id){
    std::string response = id + "_" + std::to_string(label_count);
    label_count++;
    return response;
}