#include <iostream>


int main(){
    std::string response = "";
    std::string input;
    std::cin >> input;
    char prevChar = 'a';
    for(char &c : input){
        if(c == '{'){
            if((prevChar == '}')|(prevChar == ';')){
                std::cout << " scope ";
            }
        }
        std::cout << c;
        prevChar = c;
    }
}