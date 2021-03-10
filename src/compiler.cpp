#include "top.hpp"
#include "bindings.hpp"

#include <cstdio>
#include <string>
#include <iostream>

Top* parseAST();

int main(int, char**)
{
	try
	{
		Top* ast = parseAST();
		//ast->printASM();	
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
