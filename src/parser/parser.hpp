//
// Created by user on 4/2/19.
//

#ifndef BLAHCOMPILER_PARSER_HPP
#define BLAHCOMPILER_PARSER_HPP

#include <vector>

#include <lexer.hpp>
#include "Expression.hpp"
#include "Statement.hpp"

namespace blahpiler {

using WordIt = std::vector<Word>::const_iterator;

void parse(std::vector<Word> const& words) noexcept;

std::unique_ptr<BinaryExpression> parseBinaryExpression(WordIt& wordIt,
	std::unique_ptr<Expression> lhs) noexcept;

std::unique_ptr<Expression> parseLiteralExpression(WordIt& wordIt) noexcept;

std::unique_ptr<Expression> parseIdExpression(WordIt& wordIt) noexcept;

std::unique_ptr<Entity> parseEntity(WordIt& wordIt) noexcept;

std::unique_ptr<Entity> parseFunctionDefinitionStatement(WordIt& wordIt) noexcept;

std::unique_ptr<Entity> parseValDefinitionStatement(WordIt& wordIt) noexcept;

std::unique_ptr<Expression> parseExpression(WordIt& wordIt) noexcept;

}


#endif //BLAHCOMPILER_PARSER_HPP
