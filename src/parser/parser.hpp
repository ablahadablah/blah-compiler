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

using WordIt = std::vector<Token>::const_iterator;

struct ParserContext {
	WordIt wordIt;
	WordIt endIt;
	std::vector<std::shared_ptr<Identifier>> identifiersList;
};

std::pair<std::vector<std::shared_ptr<Entity>>, std::vector<std::shared_ptr<Identifier>>> parse(std::vector<Token> const& words,
                                                                                               std::vector<std::shared_ptr<Identifier>>& identifiers) noexcept;

std::shared_ptr<BinaryExpression> parseBinaryExpression(ParserContext& parserContext,
                                                        std::shared_ptr<Expression> lhs) noexcept;

std::shared_ptr<Expression> parseLiteralExpression(ParserContext& parserContext) noexcept;

std::shared_ptr<Expression> parseIdExpression(ParserContext& parserContext) noexcept;

std::shared_ptr<Entity> parseEntity(ParserContext& parserContext) noexcept;

std::shared_ptr<Entity> parseFunctionDefinitionStatement(ParserContext& parserContext) noexcept;

std::shared_ptr<Entity> parseValDefinitionStatement(ParserContext& parserContext) noexcept;

std::shared_ptr<Entity> parseVarDefinitionStatement(ParserContext& parserContext) noexcept;

std::shared_ptr<Expression> parseExpression(ParserContext& parserContext) noexcept;

std::shared_ptr<Expression> parseAssignmentExpression(ParserContext& parserContext) noexcept;

std::shared_ptr<Entity> parseIfStatement(ParserContext& parserContext) noexcept;

std::shared_ptr<Entity> parseWhileStatement(ParserContext& parserContext) noexcept;

std::shared_ptr<Entity> parseNestedBlockStatement(ParserContext& parserContext) noexcept;

std::shared_ptr<Entity> parseReadStatement(ParserContext& parserContext) noexcept;

std::shared_ptr<Entity> parseWriteStatement(ParserContext& parserContext) noexcept;

}


#endif //BLAHCOMPILER_PARSER_HPP
