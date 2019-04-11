#include <iostream>

#include "lexer.hpp"
#include "parser/parser.hpp"
#include "translator/Translator.hpp"

int main() {
	std::cout << "Hello, World!" << std::endl;
//	std::string sourceCode = "& if && < > = while 759 == <= >= else int char val var";
	std::string sourceCode = "val someVal : int = 395 + 59 \n"
						  "val anotherVal : int = someVal + 105";

	auto parsedWords = blahpiler::parseProgram(sourceCode);

	for (auto const& word : parsedWords.first) {
		fmt::printf("parsed word %s from %d, %d, type %d\n", word.lexeme, word.lineNumber, word.posInLine,
			static_cast<int>(word.tag));
	}

	for (auto const& id : parsedWords.second) {
		fmt::printf("identifier: %s\n", id->name);
	}

	auto entities = parse(parsedWords.first, parsedWords.second);

	auto translatedCode = blahpiler::translator::translate(entities);

	fmt::printf("translated code:\n %s\n", translatedCode);

	return 0;
}