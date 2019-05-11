//
// Created by user on 3/8/19.
//

#include "lexer.hpp"

#include <cstdlib>
#include <vector>
#include <map>
#include <string>
#include <set>

#include "Identifier.hpp"

namespace blahpiler {

using namespace std::string_literals;

constexpr int charToInt(char const sym) {
	return static_cast<int>(sym) - '0';
}

KeywordTable getKeywordTable() noexcept {
	KeywordTable keywordTable;
	keywordTable["if"] = Token{0, 0, "if", Tag::IF};
	keywordTable["then"] = Token{0, 0, "then", Tag::THEN};
	keywordTable["else"] = Token{0, 0, "else", Tag::ELSE};
	keywordTable["end"] = Token{0, 0, "end", Tag::END};
	keywordTable["while"] = Token{0, 0, "while", Tag::WHILE};
	keywordTable["do"] = Token{0, 0, "do", Tag::DO};
	keywordTable["true"] = Token{0, 0, "true", Tag::TRUE};
	keywordTable["false"] = Token{0, 0, "false", Tag::FALSE};
	keywordTable["val"] = Token{0, 0, "val", Tag::VAL};
	keywordTable["var"] = Token{0, 0, "var", Tag::VAR};
	keywordTable["int"] = Token{0, 0, "int", Tag::TYPE};
	keywordTable["double"] = Token{0, 0, "double", Tag::TYPE};
	keywordTable["char"] = Token{0, 0, "char", Tag::TYPE};
	keywordTable["read"] = Token{0, 0, "read", Tag::READ};
	keywordTable["write"] = Token{0, 0, "write", Tag::WRITE};

	return keywordTable;
}

std::optional<Token> getKeyword(std::string const& lexeme,
	KeywordTable const& keywordTable) noexcept {

	if (auto keywordIt = keywordTable.find(lexeme); keywordIt != keywordTable.end()) {
		return keywordIt->second;
	} else {
		return std::nullopt;
	}
}

void handleIdentifier(Token const& word, IdentifiersTable& idsTable) noexcept {
	if (auto idIt = idsTable.find(word.lexeme); idIt == idsTable.end()) {
		auto id = std::make_shared<Identifier>();
		idsTable[word.lexeme] = id;
	}
}

// TODO: write a test for the function
// somehow return a number of checked symbols, even if the parsing was not successful
//std::optional<Token> parseNumber(char const* inputBuffer) noexcept {
std::pair<std::optional<Token>, size_t> parseNumber(std::string_view inputBuffer) noexcept {
	std::optional<Token> parsedNumber = std::nullopt;
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
		parsedNumber = Token();
		parsedNumber->tag = Tag::INT;
		parsedNumber->lexeme = lexeme;
	}
	// binary, hex, octal variations...
	else {
		// if found a symbol that's not a space, new line, b, x, then just skip it.
		do {
			lexeme += inputBuffer[bufferInd];
		} while (inputBuffer[++bufferInd] != ' ' && bufferInd < inputBuffer.size());

		parsedNumber = Token();
		parsedNumber->tag = Tag::WRONG;
		parsedNumber->lexeme = lexeme;
	}

	return std::pair(parsedNumber, bufferInd);
}

std::pair<std::optional<Token>, size_t> parseWord(std::string_view inputBuffer) noexcept {
	size_t bufferInd = 0;
	std::string lexeme;

	do {
		lexeme += inputBuffer[bufferInd];
	} while (!std::isspace(inputBuffer[++bufferInd]) && bufferInd < inputBuffer.size());

	// check if the parsed word is a keyword
	auto const keywordTable = getKeywordTable();
	auto parsedKeyword = getKeyword(lexeme, keywordTable);

	if (parsedKeyword != std::nullopt) {
		return std::pair(parsedKeyword, bufferInd);
	}

	return std::pair(Token{0, 0, lexeme, Tag::ID}, bufferInd);
}

//std::pair<std::vector<Token>, std::vector<std::shared_ptr<Identifier>>> parseProgram(
//	std::string const& inputData) noexcept {
TokensSeq parseProgram(std::string const& inputData) noexcept {
	size_t lineNumber = 0;
	size_t posInLine = 0;
	std::optional<char> peek = std::nullopt;
	size_t peekIndex = 0;
	std::string inputBuffer = inputData;
	std::vector<Token> parsedWords;
	IdentifiersTable identifiersTable;

	parsedWords.reserve(64);

	auto getch = [&] () {
		if (inputBuffer.size() == peekIndex + 1) {
			peek = std::nullopt;
		} else {
			peek = inputBuffer[++peekIndex];
		}
	};

	peek = inputBuffer.front();

	do {
		if (peek == ' ' || peek == '\t') {
			getch();
			continue;
		} else if (peek == '\n') {
			lineNumber++;
			posInLine = 0;
			fmt::printf("found a new line %d! \n", lineNumber);
			getch();
			continue;
		}

		if (peek == '<') {
			getch();

			if (peek == '=') {
				parsedWords.push_back({lineNumber, posInLine, "<="s, Tag::LE});
			} else {
				parsedWords.push_back({lineNumber, posInLine, "<", Tag::LT});
			}

			posInLine++;
		} else if (peek == '>') {
			getch();

			if (peek == '=') {
				parsedWords.push_back({lineNumber, posInLine, ">=", Tag::GE});
			} else {
				parsedWords.push_back({lineNumber, posInLine, ">", Tag::GT});
			}

			posInLine++;
		} else if (peek == '=') {
			getch();

			if (peek == '=') {
				parsedWords.push_back({lineNumber, posInLine, "==", Tag::EQ});
			} else {
				parsedWords.push_back({lineNumber, posInLine, "=", Tag::ASSIGN});
			}

			posInLine++;
		} else if (peek == '&') {
			getch();

			if (peek == '&') {
				parsedWords.push_back({lineNumber, posInLine, "&&", Tag::AND});
			} else {
				parsedWords.push_back({lineNumber, posInLine, "&", Tag::TOKEN});
			}

			posInLine++;
		} else if (peek == '|') {
			getch();

			if (peek == '|') {
				parsedWords.push_back({lineNumber, posInLine, "||", Tag::OR});
			} else {
				parsedWords.push_back({lineNumber, posInLine, "|", Tag::TOKEN});
			}

			posInLine++;
		} else if (peek == '!') {
			getch();

			if (peek == '=') {
				parsedWords.push_back({lineNumber, posInLine, "!=", Tag::NE});
			} else {
				parsedWords.push_back({lineNumber, posInLine, "!", Tag::NEG});
			}
		} else if (peek == ':') {
			parsedWords.push_back({lineNumber, posInLine, ":", Tag::COLON});
		} else if (peek == '+') {
			parsedWords.push_back({lineNumber, posInLine, "+", Tag::PLUS});
		} else if (peek == '-') {
			parsedWords.push_back({lineNumber, posInLine, "-", Tag::MINUS});
		} else if (peek == '*') {
			parsedWords.push_back({lineNumber, posInLine, "*", Tag::MUL});
		} else if (peek == '/') {
			parsedWords.push_back({lineNumber, posInLine, "/", Tag::DIV});
		} else if (peek == '(') {
			parsedWords.push_back({lineNumber, posInLine, "(", Tag::LPARENTH});
		} else if (peek == ')') {
			parsedWords.push_back({lineNumber, posInLine, ")", Tag::RPARENTH});
		} else if (peek == '{') {
			parsedWords.push_back({lineNumber, posInLine, "{", Tag::LBRACE});
		} else if (peek == '}') {
			parsedWords.push_back({lineNumber, posInLine, "}", Tag::RBRACE});
		} else if (peek == '%') {
			parsedWords.push_back({lineNumber, posInLine, "%", Tag::MOD});
		}

		if (std::isdigit(static_cast<unsigned char>(*peek))) {
			fmt::printf("trying to parse a symbol as a number %c at %d\n", inputBuffer[peekIndex], peekIndex);
			auto [parsedNumber, bytesNum] = parseNumber(&inputBuffer[peekIndex]);

			if (parsedNumber) {
				parsedNumber->lineNumber = lineNumber;
				parsedNumber->posInLine = posInLine;
				fmt::printf("a parsed number: %s, line %d, pos %d\n", parsedNumber->lexeme,
				            lineNumber, posInLine);

				parsedWords.push_back(*parsedNumber);
			} else {
				fmt::printf("Couldn't parse a number from line %d\n", lineNumber);
			}

			fmt::printf("bytes num: %d\n", bytesNum);
			peekIndex += bytesNum;
			posInLine += bytesNum;
		}

		if (std::isalpha(*peek)) {
			auto [parsedWord, bytesNum] = parseWord(&inputBuffer[peekIndex]);

			if (parsedWord == std::nullopt) {
				fmt::printf("Something went wrong when parsing a word at: %d, %d", peekIndex, posInLine);

				peekIndex += bytesNum;
				posInLine += bytesNum;
				continue;
			}

			parsedWord->lineNumber = lineNumber;
			parsedWord->posInLine = posInLine;
			peekIndex += bytesNum;
			posInLine += bytesNum;

			if (parsedWord->tag == Tag::ID) {
				handleIdentifier(*parsedWord, identifiersTable);
			}

			parsedWords.push_back(*parsedWord);
		}

		getch();
		posInLine++;
		// end of the input stream, finish parsing
	} while (peek != std::nullopt && peek != '\0');

//	return std::make_pair(parsedWords, std::move(identifiersList));
	return TokensSeq{parsedWords, std::move(identifiersTable)};
}

}
