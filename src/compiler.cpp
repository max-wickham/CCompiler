#include "top.hpp"
#include "bindings.hpp"

#include <cstdio>
#include <string>

Top* parseAST();

int main(int, char**)
{
	try
	{
		Top* ast(parseAST());
		ast->printASM();	
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
