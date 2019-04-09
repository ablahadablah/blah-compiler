#include "parser.hpp"

#include <memory>
#include <string>

namespace blahpiler {

void parse(std::vector<Word> const& words,
	std::vector<std::unique_ptr<Identifier>>& identifiers) noexcept {
	fmt::printf("parsing the program\n");

	std::vector<std::unique_ptr<Entity>> entities;
	ParserContext parserContext;
	parserContext.wordIt = words.begin();
	parserContext.endIt = words.end();
	parserContext.identifiersList = std::move(identifiers);

	for (; parserContext.wordIt != parserContext.endIt; parserContext.wordIt += 1) {
		entities.push_back(parseEntity(parserContext));
	}

	fmt::printf("Entities num: %d\n", entities.size());
}

std::unique_ptr<BinaryExpression> parseBinaryExpression(ParserContext& parserContext,
                                                        std::unique_ptr<Expression> lhs) noexcept {
	fmt::printf("Parsing a binary expression\n");
	auto expr = std::make_unique<BinaryExpression>();

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

std::unique_ptr<Expression> parseLiteralExpression(ParserContext& parserContext) noexcept {
	fmt::printf("parsing a literal expression\n");

	switch (parserContext.wordIt->tag) {
		case Tag::INT:
			return std::unique_ptr<Expression>(new IntLiteralExpression(std::stoi(parserContext.wordIt->lexeme)));
		case Tag::DOUBLE:
			return std::make_unique<DoubleLiteralExpression>(std::stod(parserContext.wordIt->lexeme));
		default:
			fmt::printf("Some wrong tag, cannot parse a literal expression\n");
			return nullptr;
	}
}

std::unique_ptr<Entity> parseEntity(ParserContext& parserContext) noexcept {
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

std::unique_ptr<Entity> parseFunctionDefinitionStatement(ParserContext& parserContext) noexcept {
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

std::unique_ptr<Entity> parseValDefinitionStatement(ParserContext& parserContext) noexcept {
	fmt::printf("parsing a val definition\n");
	auto valDefinitionStmt = std::make_unique<ValDefinitionStatement>();

	parserContext.wordIt++;
	if (parserContext.wordIt->tag != Tag::ID) {
		fmt::printf("Couldn't parse val definition at line %d: expected an ID\n", parserContext.wordIt->lineNumber);
		return nullptr;
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

std::unique_ptr<Expression> parseExpression(ParserContext& parserContext) noexcept {
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

	std::unique_ptr<Expression> expr = nullptr;

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
			return parseIdExpression(parserContext);
		default:
			fmt::printf("Couldn't parse an expression at line %d: wrong tag\n", parserContext.wordIt->lineNumber);
			return nullptr;
	}
}

std::unique_ptr<Expression> parseIdExpression(ParserContext& parserContext) noexcept {
	return nullptr;
}

}
