#include <iostream>

#include "lexer.hpp"

int main()
{
	std::cout << "Hello, World!" << std::endl;
	std::string sourceCode = "12345 5432\n92blah 12";

	blahpiler::parseProgram(sourceCode);

	return 0;
}