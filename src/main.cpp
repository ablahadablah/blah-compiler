#include <iostream>

#include "lexer.hpp"

int main()
{
	std::cout << "Hello, World!" << std::endl;
	std::string sourceCode = "& if && < > = while 759 == <= >= else || 234 &&";

	auto parsedWords = blahpiler::parseProgram(sourceCode);

	for (auto const& word : parsedWords) {
		fmt::printf("parsed word %s from %d, %d\n", word.lexeme, word.lineNumber, word.posInLine);
	}

	return 0;
}