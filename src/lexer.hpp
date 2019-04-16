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

#include "Identifier.hpp"

namespace blahpiler {

enum class Tag {
	ID = 468,
	OR, AND, TRUE, FALSE, INT, DOUBLE, CHAR, WRONG,
	GT, LT, GE, LE, EQ, NE, ASSIGN, TOKEN, IF, ELSE, WHILE,
	LBRACE, RBRACE, LPARENTH, RPARENTH, FUN, COLON, VAL, VAR,
	TYPE, PLUS, MINUS, DIV, MUL, MOD, NEG
};

struct Token {
	size_t lineNumber;
	size_t posInLine;
	std::string lexeme;
	Tag tag;
	std::optional<size_t> identifierIndex;
};

using KeywordTable = std::map<std::string, Token>;

KeywordTable getKeywordTable() noexcept;

std::optional<Token> getKeyword(std::string const& lexeme,
	KeywordTable const& keywordTable) noexcept;

//std::optional<Token> parseNumber(char const* inputBuffer) noexcept;
std::pair<std::optional<Token>, size_t> parseNumber(std::string_view inputBuffer) noexcept;

std::pair<std::vector<Token>, std::vector<std::shared_ptr<Identifier>>> parseProgram(
	std::string const& inputData) noexcept;

}

#endif //BLAHCOMPILER_LEXER_HPP
