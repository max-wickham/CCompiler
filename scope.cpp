#include <iostream>


int main(){
    std::string response = "";
    std::string input;
    char prevChar = 'a';
    while(std::getline(std::cin, input)){
        //std::cout << input <<std::endl;
        for(char &c : input){
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