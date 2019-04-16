#include "parser.hpp"

#include <memory>
#include <string>
#include <algorithm>

namespace blahpiler {

std::pair<std::vector<std::shared_ptr<Entity>>, std::vector<std::shared_ptr<Identifier>>> parse(std::vector<Token> const& words,
                                                                                               std::vector<std::shared_ptr<Identifier>>& identifiers) noexcept {
	fmt::printf("parsing the program\n");

	std::vector<std::shared_ptr<Entity>> entities;
	ParserContext parserContext;
	parserContext.wordIt = words.begin();
	parserContext.endIt = words.end();
	parserContext.identifiersList = std::move(identifiers);

	for (; parserContext.wordIt != parserContext.endIt; parserContext.wordIt += 1) {
		entities.push_back(parseEntity(parserContext));
	}

	fmt::printf("Entities num: %d\n", entities.size());

	return std::make_pair(entities, parserContext.identifiersList);
}

std::shared_ptr<BinaryExpression> parseBinaryExpression(ParserContext& parserContext,
                                                        std::shared_ptr<Expression> lhs) noexcept {
	fmt::printf("Parsing a binary expression\n");
	auto expr = std::make_shared<BinaryExpression>();

	switch (parserContext.wordIt->tag) {
		case Tag::OR:
		case Tag::AND:
		case Tag::ASSIGN:
		case Tag::EQ:
		case Tag::GE:
		case Tag::GT:
		case Tag::LE:
		case Tag::LT:
		case Tag::PLUS:
		case Tag::MINUS:
		case Tag::MUL:
		case Tag::DIV:
			expr->lhs = std::move(lhs);
			expr->operatorTag = parserContext.wordIt->tag;
			parserContext.wordIt++;
			expr->rhs = std::move(parseExpression(parserContext));

			return expr;
		default:
			return nullptr;
	}
}

std::shared_ptr<Expression> parseLiteralExpression(ParserContext& parserContext) noexcept {
	fmt::printf("parsing a literal expression\n");

	switch (parserContext.wordIt->tag) {
		case Tag::INT:
			return std::shared_ptr<Expression>(new IntLiteralExpression(std::stoi(parserContext.wordIt->lexeme)));
		case Tag::DOUBLE:
			return std::make_shared<DoubleLiteralExpression>(std::stod(parserContext.wordIt->lexeme));
		default:
			fmt::printf("Some wrong tag, cannot parse a literal expression\n");
			return nullptr;
	}
}

std::shared_ptr<Entity> parseEntity(ParserContext& parserContext) noexcept {
	fmt::printf("Parsing an entity\n");

	switch (parserContext.wordIt->tag) {
		case Tag::OR:
		case Tag::AND:
		case Tag::ASSIGN:
		case Tag::EQ:
		case Tag::GE:
		case Tag::GT:
		case Tag::LE:
		case Tag::LT:
			// need to somehow access the previous token
//			return parseBinaryExpression(wordIt);
			return nullptr;
		case Tag::INT:
			return parseLiteralExpression(parserContext);
		case Tag::FUN:
			return parseFunctionDefinitionStatement(parserContext);
		case Tag::VAL:
			return parseValDefinitionStatement(parserContext);
		default:
			fmt::printf("blah blah blah default: %s\n", parserContext.wordIt->lexeme);
			return nullptr;
	}
}

std::shared_ptr<Entity> parseFunctionDefinitionStatement(ParserContext& parserContext) noexcept {
	parserContext.wordIt++;
	if (parserContext.wordIt->tag != Tag::ID) {
		fmt::printf("Couldn't parse function definition at line %d\n", parserContext.wordIt->lineNumber);
		return nullptr;
	}
	parserContext.wordIt++;
	if (parserContext.wordIt->tag != Tag::LPARENTH) {
		fmt::printf("Couldn't parse function definition at line %d\n", parserContext.wordIt->lineNumber);
		return nullptr;
	}
	parserContext.wordIt++;
	if (parserContext.wordIt->tag != Tag::RPARENTH) {
		fmt::printf("Couldn't parse function definition at line %d\n", parserContext.wordIt->lineNumber);
		return nullptr;
	}
	parserContext.wordIt++;
	if (parserContext.wordIt->tag != Tag::COLON) {
		fmt::printf("Couldn't parse function definition at line %d\n", parserContext.wordIt->lineNumber);
		return nullptr;
	}
	parserContext.wordIt++;
	if (parserContext.wordIt->tag != Tag::LBRACE) {
		fmt::printf("Couldn't parse function definition at line %d\n", parserContext.wordIt->lineNumber);
		return nullptr;
	}
	parserContext.wordIt++;
	if (parserContext.wordIt->tag != Tag::RBRACE) {
		fmt::printf("Couldn't parse function definition at line %d\n", parserContext.wordIt->lineNumber);
		return nullptr;
	}

	return nullptr;
}

std::shared_ptr<Entity> parseValDefinitionStatement(ParserContext& parserContext) noexcept {
	fmt::printf("parsing a val definition\n");
	auto valDefinitionStmt = std::make_shared<ValDefinitionStatement>();

	parserContext.wordIt++;
	if (parserContext.wordIt->tag != Tag::ID) {
		fmt::printf("Couldn't parse val definition at line %d: expected an ID\n", parserContext.wordIt->lineNumber);
		return nullptr;
	}
	for (size_t i = 0; i < parserContext.identifiersList.size(); i++) {
		if (parserContext.identifiersList[i]->name == parserContext.wordIt->lexeme) {
			valDefinitionStmt->idIndex = i;
			fmt::printf("val definition statemt of an id num %d\n", i);
			break;
		}
	}
	valDefinitionStmt->name = parserContext.wordIt->lexeme;

	parserContext.wordIt++;
	if (parserContext.wordIt->tag != Tag::COLON) {
		fmt::printf("Couldn't parse val definition at line %d: expected a colon\n", parserContext.wordIt->lineNumber);
		return nullptr;
	}
	parserContext.wordIt++;
	if (parserContext.wordIt->tag != Tag::TYPE) {
		fmt::printf("Couldn't parse val at line %d: expected a type\n", parserContext.wordIt->lineNumber);
		return nullptr;
	}
	valDefinitionStmt->type = parserContext.wordIt->lexeme;

	parserContext.wordIt++;
	if (parserContext.wordIt->tag != Tag::ASSIGN) {
		fmt::printf("Couldn't parse val definition at line %d: expected an assignment \n", parserContext.wordIt->lineNumber);
		return nullptr;
	}

	parserContext.wordIt++;
	auto initExpression = parseExpression(parserContext);

	if (initExpression == nullptr) {
		fmt::printf("Couldn't parse val definition at line %d: expected an init expression \n", parserContext.wordIt->lineNumber);
		return nullptr;
	}

	valDefinitionStmt->assignExpression = std::move(initExpression);

	return valDefinitionStmt;
}

std::shared_ptr<Expression> parseExpression(ParserContext& parserContext) noexcept {
	fmt::printf("parsing an expression\n");

	auto const checkForBinaryExpr = [] (WordIt& wordIt) -> bool {
		switch (wordIt->tag) {
			case Tag::OR:
			case Tag::AND:
			case Tag::ASSIGN:
			case Tag::EQ:
			case Tag::GE:
			case Tag::GT:
			case Tag::LE:
			case Tag::LT:
			case Tag::PLUS:
			case Tag::MINUS:
			case Tag::MUL:
			case Tag::DIV:
				return true;
			default:
				return false;
		}
	};

	std::shared_ptr<Expression> expr = nullptr;

	switch (parserContext.wordIt->tag) {
		case Tag::OR:
		case Tag::AND:
		case Tag::ASSIGN:
		case Tag::EQ:
		case Tag::GE:
		case Tag::GT:
		case Tag::LE:
		case Tag::LT:
			// need to somehow access the previous token
//			return parseBinaryExpression(wordIt);
			return nullptr;
		case Tag::INT:
			expr = std::move(parseLiteralExpression(parserContext));

			if (auto tmpIt = parserContext.wordIt + 1; checkForBinaryExpr(tmpIt)) {
				parserContext.wordIt++;
				return parseBinaryExpression(parserContext, std::move(expr));
			} else {
				return expr;
			}
		case Tag::ID:
			expr = std::move(parseIdExpression(parserContext));

			if (auto tmpIt = parserContext.wordIt + 1; checkForBinaryExpr(tmpIt)) {
				parserContext.wordIt++;
				return parseBinaryExpression(parserContext, std::move(expr));
			} else {
				return expr;
			}
		default:
			fmt::printf("Couldn't parse an expression at line %d: wrong tag\n", parserContext.wordIt->lineNumber);
			return nullptr;
	}
}

std::shared_ptr<Expression> parseIdExpression(ParserContext& parserContext) noexcept {
	fmt::printf("parsing an id expression\n");

	for (size_t i = 0; i < parserContext.identifiersList.size(); i++) {
		if (parserContext.identifiersList[i]->name == parserContext.wordIt->lexeme) {
			auto idExpr = std::make_shared<IdExpression>();
			idExpr->idListIndex = i;

			return idExpr;
		}
	}

	fmt::printf("identifier is not defined: %s\n", parserContext.wordIt->lexeme);

	return nullptr;
}

}
