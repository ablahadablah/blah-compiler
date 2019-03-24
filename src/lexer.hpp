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
#include <map>
#include <vector>

#include <fmt/printf.h>

namespace blahpiler {

enum class Tag {
	ID = 468,
	OR, AND, TRUE, FALSE, INT, DOUBLE, CHAR, WRONG,
	GT, LT, GE, LE, EQ, ASSIGN, TOKEN, IF, ELSE, WHILE,
	LBRACE, RBRACE, LPARENTH, RPARENTH
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

using KeywordTable = std::map<std::string, Word>;

KeywordTable getKeywordTable() noexcept;

std::optional<Word> getKeyword(std::string const& lexeme,
	KeywordTable const& keywordTable) noexcept;

//std::optional<Word> parseNumber(char const* inputBuffer) noexcept;
std::pair<std::optional<Word>, size_t> parseNumber(std::string_view inputBuffer) noexcept;

std::vector<Word> parseProgram(std::string const& inputData) noexcept;

}

#endif //BLAHCOMPILER_LEXER_HPP
