//
// Created by user on 3/8/19.
//

#include "lexer.hpp"

#include <cstdlib>
#include <vector>

namespace blahpiler {

constexpr int charToInt(char const sym) {
	return static_cast<int>(sym) - '0';
}

// TODO: write a test for the function
// somehow return a number of checked symbols, even if the parsing was not successful
//std::optional<Word> parseNumber(char const* inputBuffer) noexcept {
std::pair<std::optional<Word>, size_t> parseNumber(std::string_view inputBuffer) noexcept {
	std::optional<Word> parsedNumber = std::nullopt;
	size_t bufferInd = 0;
	int value = 0;
	std::string lexeme;

	// first character is already a digit
	do {
		value = value * 10 + charToInt(inputBuffer[bufferInd]);
		lexeme += inputBuffer[bufferInd];
	} while (std::isdigit(inputBuffer[++bufferInd]) && bufferInd < inputBuffer.size());

	auto symbol = inputBuffer[bufferInd];

	if (symbol == '.') {
		// parse as a double
	} else if (symbol == ' ' || symbol == '\n' || symbol == '\0') {
		// return an int
		parsedNumber = Word();
		parsedNumber->tag = Tag::INT;
		parsedNumber->lexeme = lexeme;
	}
	// binary, hex, octal variations...
	else {
		// if found a symbol that's not a space, new line, b, x, then just skip it.
		do {
			lexeme += inputBuffer[bufferInd];
		} while (inputBuffer[++bufferInd] != ' ' && bufferInd < inputBuffer.size());

		parsedNumber = Word();
		parsedNumber->tag = Tag::WRONG;
		parsedNumber->lexeme = lexeme;
	}

	return std::pair(parsedNumber, bufferInd);
}

void parseProgram(std::string const& inputData) noexcept {
	size_t lineNumber = 0;
	size_t posInLine = 0;
	std::optional<char> peek = std::nullopt;
	size_t peekIndex = 0;
	std::string inputBuffer = inputData;
	std::vector<Word> parsedWords;

	auto parse = [&] () {
		auto getch = [&] () {
			if (inputBuffer.size() == peekIndex + 1) {
				peek = std::nullopt;
			} else {
				posInLine++;
				peek = inputBuffer[++peekIndex];
			}
		};

		peek = inputBuffer.front();

		do {
			if (peek == ' ' || peek == '\t') {
				continue;
			} else if (peek == '\n') {
				lineNumber++;
				posInLine = 0;
				fmt::printf("found a new line %d! \n", lineNumber);
				continue;
			}

			if (peek == '<') {
				getch();

				if (peek == '=') {
					parsedWords.emplace_back(lineNumber, posInLine, "<=", Tag::LE);
				} else {
					parsedWords.emplace_back(lineNumber, posInLine, "<", Tag::LT);
				}
			} else if (peek == '>') {
				getch();

				if (peek == '=') {
					parsedWords.emplace_back(lineNumber, posInLine, ">=", Tag::GE);
				} else {
					parsedWords.emplace_back(lineNumber, posInLine, ">", Tag::GT);
				}
			} else if (peek == '=') {
				getch();

				if (peek == '=') {
					parsedWords.emplace_back(lineNumber, posInLine, "==", Tag::EQ);
				} else {
					parsedWords.emplace_back(lineNumber, posInLine, "=", Tag::ASSIGN);
				}
			} else if (peek == '&') {
				getch();

				if (peek == '&') {
					parsedWords.emplace_back(lineNumber, posInLine, "==", Tag::EQ);
				} else {
					parsedWords.emplace_back(lineNumber, posInLine, "=", Tag::ASSIGN);
				}
			} else if (peek == '=') {
				getch();

				if (peek == '=') {
					parsedWords.emplace_back(lineNumber, posInLine, "==", Tag::EQ);
				} else {
					parsedWords.emplace_back(lineNumber, posInLine, "=", Tag::ASSIGN);
				}
			}

			if (std::isdigit(static_cast<unsigned char>(*peek))) {
				fmt::printf("trying to parse a symbol as a number %c at %d\n", inputBuffer[peekIndex], peekIndex);
				auto [parsedNumber, bytesNum] = parseNumber(&inputBuffer[peekIndex]);

				if (parsedNumber) {
					parsedNumber->lineNumber = lineNumber;
					parsedNumber->posInLine = posInLine;
					fmt::printf("a parsed number: %s, line %d, pos %d\n", parsedNumber->lexeme,
						lineNumber, posInLine);
				} else {
					fmt::printf("Couldn't parse a number from line %d\n", lineNumber);
				}

				fmt::printf("bytes num: %d\n", bytesNum);
				peekIndex += bytesNum;
				posInLine += bytesNum;
			}

			getch();
			// end of the input stream, finish parsing
		} while (peek != std::nullopt);
	};

	parse();
}


Lexer::Lexer(std::string newInputBuffer)
	: inputBuffer(std::move(newInputBuffer)) {}

void Lexer::parse() noexcept {

}

bool Lexer::getch(char s) noexcept
{
	return false;
}

void Lexer::getch() noexcept
{
	if (inputBuffer.size() == peekIndex + 1) {
		peek = std::nullopt;
	} else {
		peek = inputBuffer[++peekIndex];
	}
}

}
