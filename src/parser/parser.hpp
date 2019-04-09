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

struct ParserContext {
	WordIt wordIt;
	WordIt endIt;
	std::vector<std::unique_ptr<Identifier>> identifiersList;
};

void parse(std::vector<Word> const& words,
	std::vector<std::unique_ptr<Identifier>>& identifiers) noexcept;

std::unique_ptr<BinaryExpression> parseBinaryExpression(ParserContext& parserContext,
                                                        std::unique_ptr<Expression> lhs) noexcept;

std::unique_ptr<Expression> parseLiteralExpression(ParserContext& parserContext) noexcept;

std::unique_ptr<Expression> parseIdExpression(ParserContext& parserContext) noexcept;

std::unique_ptr<Entity> parseEntity(ParserContext& parserContext) noexcept;

std::unique_ptr<Entity> parseFunctionDefinitionStatement(ParserContext& parserContext) noexcept;

std::unique_ptr<Entity> parseValDefinitionStatement(ParserContext& parserContext) noexcept;

std::unique_ptr<Expression> parseExpression(ParserContext& parserContext) noexcept;

}


#endif //BLAHCOMPILER_PARSER_HPP
