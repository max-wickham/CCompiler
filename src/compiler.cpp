#include "top.hpp"
#include "bindings.hpp"

#include <cstdio>
#include <string>
#include <iostream>

Top* parseAST();

int main(int argc, char *argv[])
{
	// std::string fileIn;
	// std::string fileOut;
	// for (int i = 0; i < argc; ++i) {
	// 	std::string arg = argv[i];
	// 	if(arg == "-S"){
	// 		fileIn = argv[i+1];
	// 	}
	// 	if(arg == "-o"){
	// 		fileOut = argv[i+1];
	// 	}
	// }
  
	try
	{
		Top* ast = parseAST();
		ast->printASM();	
		//std::cout << "hello";
	}
	catch(const std::exception& e)
	{
		fprintf(stderr, "%s\n", e.what());
	}
	catch(...)
	{
		fprintf(stderr, "Error : Exception thrown\n");
	}

	return 0;
}
