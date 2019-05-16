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

struct EntitiesSeq final {
	std::vector<std::shared_ptr<Entity>> entities;
	IdentifiersTable idsTable;
};

struct ParserContext final {
	WordIt wordIt;
	WordIt endIt;
	std::vector<std::shared_ptr<Identifier>> identifiersList;
	IdentifiersTable idsTable;

	Token const& nextToken() {
		return *(++wordIt);
	}

	Token const& getToken() {
		return *wordIt;
	}
};

//std::pair<std::vector<std::shared_ptr<Entity>>, std::vector<std::shared_ptr<Identifier>>> parse(std::vector<Token> const& words,
//                                                                                               std::vector<std::shared_ptr<Identifier>>& identifiers) noexcept;

EntitiesSeq parse(TokensSeq& tokensSeq) noexcept;

std::shared_ptr<BinaryExpression> parseBinaryExpression(ParserContext& parserContext,
                                                        std::shared_ptr<Expression> lhs) noexcept;

std::shared_ptr<Expression> parseLiteralExpression(ParserContext& parserContext) noexcept;

std::shared_ptr<Expression> parseIdExpression(ParserContext& ctx) noexcept;

std::shared_ptr<Entity> parseEntity(ParserContext& parserContext) noexcept;

std::shared_ptr<Entity> parseFunctionDefinitionStatement(ParserContext& parserContext) noexcept;

std::shared_ptr<Entity> parseValDefinitionStatement(ParserContext& parseCtx) noexcept;

std::shared_ptr<Entity> parseVarDefinitionStatement(ParserContext& ctx) noexcept;

std::shared_ptr<Expression> parseExpression(ParserContext& parserContext) noexcept;

std::shared_ptr<Expression> parseAssignmentExpression(ParserContext& ctx) noexcept;

std::shared_ptr<Entity> parseIfStatement(ParserContext& parserContext) noexcept;

std::shared_ptr<Entity> parseWhileStatement(ParserContext& parserContext) noexcept;

std::shared_ptr<Entity> parseNestedBlockStatement(ParserContext& parserContext) noexcept;

std::shared_ptr<Entity> parseReadStatement(ParserContext& parserContext) noexcept;

std::shared_ptr<Entity> parseWriteStatement(ParserContext& parserContext) noexcept;

std::shared_ptr<Expression> parseArrayInitExpr(ParserContext& parserContext) noexcept;

}


#endif //BLAHCOMPILER_PARSER_HPP
