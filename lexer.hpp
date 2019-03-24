#include <utility>

//
// Created by user on 3/8/19.
//

#ifndef BLAHCOMPILER_LEXER_HPP
#define BLAHCOMPILER_LEXER_HPP

#include <cctype>
#include <string>
#include <optional>
#include <string_view>
#include <utility>

#include <fmt/printf.h>

namespace blahpiler {

enum class Tag {
	ID = 468,
	OR, AND, TRUE, FALSE, INT, DOUBLE, CHAR, WRONG,
	GT, LT, GE, LE, EQ, ASSIGN
};

struct Token {
	std::string symbol;
};

struct Word {
	size_t lineNumber;
	size_t posInLine;
	std::string lexeme;
	Tag tag;
};

//std::optional<Word> parseNumber(char const* inputBuffer) noexcept;
std::pair<std::optional<Word>, size_t> parseNumber(std::string_view inputBuffer) noexcept;

void parseProgram(std::string const& inputData) noexcept;

class Lexer {
	int lineNumber = 0;
	std::optional<char> peek = std::nullopt;
	size_t peekIndex = 0;

	std::string inputBuffer;

public:
	explicit Lexer(std::string newInputBuffer);

	void parse() noexcept;

private:
	void getch() noexcept;
	bool getch(char s) noexcept;
};

}

#endif //BLAHCOMPILER_LEXER_HPP
