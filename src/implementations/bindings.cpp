#include <map>
#include <vector>
#include <string>
#include <iostream>

#include "type.hpp"
#include "decleration.hpp"
#include "bindings.hpp"

Bindings::Bindings(){
    std::map<std::string,BindingData> map;
    bindings.push_front(map);
}

Bindings::Bindings(std::map<std::string, Type*> *functionsP){
    this->functions = functionsP;
}

Bindings* Bindings::createGlobalBindings(){
    //Type * type = functions->at("y");
    Bindings *newBindings = new Bindings();
    std::list<std::map<std::string, BindingData>>::iterator it;
    newBindings->bindings = bindings;
    newBindings->structs = structs;
    newBindings->current_offset = this->current_offset;
    newBindings->functions = functions;
    //std::cout << newBindings->functions << std::endl;
    //std::cout << functions << std::endl;
    //std::cout << "Get Function" << std::endl;
    return newBindings;
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

int Bindings::elementPosition(std::string structId, std::string elementId){
    int structOffset = this->stackPosition(structId);
    structId = ((Struct*)this->getVariable(structId))->getName();
    int elementOffset = structs.at(structId).elements.at(elementId).offset;
    return structOffset + elementOffset;
}

void Bindings::addVariable(Decleration *decleration){
    BindingData data;
    data.type = decleration->type;
    data.size = decleration->type->getSize();
    data.offset = current_offset;
    //std::cout << "cureent name " << decleration->id << std::endl;
    //std::cout << "cureent value " << current_offset << std::endl;
    current_offset -= data.size;
    //std::cout << "cureent offset " << current_offset << std::endl;
    bindings.begin()->insert(std::pair<std::string,BindingData>(decleration->id,data));
}

void Bindings::addFunction(std::string id, Type *type){
    //std::cout << "Add Function" << id << std::endl;
    functions->insert(std::pair<std::string,Type*>(id,type));
}

void Bindings::addStruct(std::string id, StructDefinition *structDefinition){
    BindingsStruct bindingsStruct;
    int totaloffset = 0;
    for(StructEntry *entry: structDefinition->elements){
        BindingData data;
        data.type = entry->type;
        data.size = entry->type->getSize();
        data.offset = totaloffset;
        totaloffset -= data.size;
        //std::cout << "addign entry " << entry->id << std::endl;
        bindingsStruct.elements[entry->id] = data;
    }
    Struct *structT = new Struct(totaloffset * -1,&id);
    bindingsStruct.structT = structT;  
    //std::cout << "addign struct " << id << std::endl;
    structs[structDefinition->id] = bindingsStruct;
    //std::cout << "added struct bindings" <<std::endl;
}

Type* Bindings::getFunction(std::string id){
    return functions->at(id);
}

Type* Bindings::getVariable(std::string id){
    //std::cout << "get variable " << id << std::endl;
    std::list<std::map<std::string, BindingData>>::iterator it;
    for (it = bindings.begin(); it != bindings.end(); ++it){
        if(it->find(id) != it->end()){
            return it->at(id).type;
        }
    }
    //std::cout << "got variable " << std::endl;
    return 0;
}

Type* Bindings::getElement(std::string structId, std::string elementId){
    structId = ((Struct*)this->getVariable(structId))->getName();
    BindingsStruct bin = structs.at(structId);
    Type *type = structs.at(structId).elements.at(elementId).type;
    return type;
}

Struct* Bindings::getStruct(std::string id){
        //std::cout << "get variable " << id << std::endl;
    return structs.at(id).structT;
}

void Bindings::setBreak(std::string label){
    break_label.push_front(label);
}

void Bindings::setContinue(std::string label){
    continue_label.push_front(label);
}

void Bindings::setCase(std::string label){
    case_label.push_front(label);
}

std::string Bindings::getBreak(){
    std::string label = *break_label.begin();
    return label;
}

std::string Bindings::getContinue(){
    std::string label = *continue_label.begin();
    return label;
}

std::string Bindings::getCase(){
    std::string label = *case_label.begin();
    return label;
}

void Bindings::removeBreak(){
    break_label.pop_front();
}

void Bindings::removeContinue(){
    continue_label.pop_front();
}

void Bindings::removeCase(){
    case_label.pop_front();
}

std::string Bindings::createLabel(std::string id){
    std::string response = id + "_" + std::to_string(label_count);
    label_count++;
    return response;
}