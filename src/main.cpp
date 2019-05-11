#include <iostream>
#include <fstream>

#include "lexer.hpp"
#include "parser/parser.hpp"
#include "translator/Translator.hpp"

std::string readSourceFile(std::string const& fileName) {
    std::ifstream in(fileName, std::ios::in);

    if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();

		return(contents);
    }

    fmt::printf("Couldn't read source code from the file: %s\n", fileName);

    return "";
}

void writeTranslatedCode(std::string const& fileName, std::string const& code) {
	std::ofstream out(fileName);

	if (out) {
		out << code;
		out.close();
	} else {
		fmt::printf("Couldn't open the file: %s\n", fileName);
	}
}

int main(int argc, char* argv[]) {
	std::cout << "Hello, World!" << std::endl;

	if (argc == 1) {
		fmt::printf("Error: specify input file\n");
		return 1;
	}

	std::string targetFile = "out.cpp";

	if (argc == 3) {
		targetFile = argv[2];
	}

	auto const sourceCode = readSourceFile(argv[1]);
	auto tokensSeq = blahpiler::parseProgram(sourceCode);

	for (auto const& word : tokensSeq.tokens) {
		fmt::printf("parsed word %s from %d, %d, type %d\n", word.lexeme, word.lineNumber, word.posInLine,
			static_cast<int>(word.tag));
	}

	for (auto const& id : tokensSeq.idsTable) {
		fmt::printf("identifier: %s\n", id.first);
	}

	auto entitiesSeq = parse(tokensSeq);
	auto translatedCode = blahpiler::translator::translate(entitiesSeq);

	fmt::printf("translated code:\n %s\n", translatedCode);

	writeTranslatedCode(targetFile, translatedCode);

	return 0;
}