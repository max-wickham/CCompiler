#include <iostream>


int main(){
    std::string response = "";
    std::string input;
    char prevChar = 'a';
    while(std::getline(std::cin, input)){
        //std::cout << input <<std::endl;
        for(int i = 0; i < input.size(); i ++){//
        char c = input[i];//
        if(i < input.size() - 6){
            if(input.substr(i,6) == " case "){
                if(prevChar != '{'){
                    std::cout << "}";
                }
            }
        }
        if(i < input.size() - 9){
            if(input.substr(i,9) == " default "){
                if(prevChar != '{'){
                    std::cout << "}";
                }
            }
        }
        if(i < input.size() - 9){
            if(input.substr(i,9) == " default\t"){
                if(prevChar != '{'){
                    std::cout << "}";
                }
            }
        }
        if(i < input.size() - 9){
            if(input.substr(i,9) == " default:"){
                if(prevChar != '{'){
                    std::cout << "}";
                }
            }
        }
        ////for(char &c : input){
            if(c == '{'){
                if((prevChar == '}')|(prevChar == ';')){
                    std::cout << "scope ";
                }
            }
            std::cout << c;
            if((c != '\t')&(c!=' ')&(c!='\n')&(c!='\v')&(c!='\r')){
                //std::cout << prevChar;
                //std::cout << c <<std::endl;
                prevChar = c;
            }
        }
        std::cout << std::endl;
    }
}