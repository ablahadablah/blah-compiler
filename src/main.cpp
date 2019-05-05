#include <iostream>

#include "lexer.hpp"
#include "parser/parser.hpp"
#include "translator/Translator.hpp"

int main() {
	std::cout << "Hello, World!" << std::endl;
//	std::string sourceCode = "& if && < > = while 759 == <= >= else int char val var";
	std::string sourceCode = "var someVal : int = 0 \n"
	                         "read someVal \n"
						  "val anotherVal : int = someVal + 105 \n"
						  "write anotherVal \n";

	auto tokensSeq = blahpiler::parseProgram(sourceCode);

	for (auto const& word : tokensSeq.tokens) {
		fmt::printf("parsed word %s from %d, %d, type %d\n", word.lexeme, word.lineNumber, word.posInLine,
			static_cast<int>(word.tag));
	}

	for (auto const& id : tokensSeq.idsTable) {
		fmt::printf("identifier: %s\n", id.first);
	}

	auto entitiesSeq = parse(tokensSeq);
//
	auto translatedCode = blahpiler::translator::translate(entitiesSeq);
//
	fmt::printf("translated code:\n %s\n", translatedCode);

	return 0;
}