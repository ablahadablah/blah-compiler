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
		case Tag::MOD:
		case Tag::NE:
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
			fmt::printf("returning an int literal expr\n");
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
		case Tag::PLUS:
		case Tag::MINUS:
		case Tag::MUL:
		case Tag::DIV:
		case Tag::MOD:
		case Tag::NE:
			// need to somehow access the previous token
//			return parseBinaryExpression(wordIt);
			return nullptr;
		case Tag::INT:
			return parseLiteralExpression(parserContext);
		case Tag::FUN:
			return parseFunctionDefinitionStatement(parserContext);
		case Tag::VAL:
			return parseValDefinitionStatement(parserContext);
		case Tag::VAR:
			return parseVarDefinitionStatement(parserContext);
		case Tag::ID:
			return parseAssignmentExpression(parserContext);
		case Tag::IF:
			return parseIfStatement(parserContext);
		case Tag::WHILE:
			return parseWhileStatement(parserContext);
		case Tag::LBRACE:
			return parseNestedBlockStatement(parserContext);
		case Tag::READ:
			return parseReadStatement(parserContext);
		case Tag::WRITE:
			return parseWriteStatement(parserContext);
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

std::shared_ptr<Entity> parseVarDefinitionStatement(ParserContext& parserContext) noexcept {
	fmt::printf("parsing a var definition\n");
	auto varDefinitionStmt = std::make_shared<VarDefinitionStatement>();

	parserContext.wordIt++;
	if (parserContext.wordIt->tag != Tag::ID) {
		fmt::printf("Couldn't parse var definition at line %d: expected an ID\n", parserContext.wordIt->lineNumber);
		return nullptr;
	}

	for (size_t i = 0; i < parserContext.identifiersList.size(); i++) {
		if (parserContext.identifiersList[i]->name == parserContext.wordIt->lexeme) {
			varDefinitionStmt->idIndex = i;
			fmt::printf("var definition statement of an id num %d\n", i);
			break;
		} else {
			fmt::printf("Couldn't find an identifier: %s\n", parserContext.wordIt->lexeme);
			return nullptr;
		}
	}

	parserContext.wordIt++;
	if (parserContext.wordIt->tag != Tag::COLON) {
		fmt::printf("Couldn't parse var definition at line %d: expected a colon\n", parserContext.wordIt->lineNumber);
		return nullptr;
	}
	parserContext.wordIt++;
	if (parserContext.wordIt->tag != Tag::TYPE) {
		fmt::printf("Couldn't parse var at line %d: expected a type\n", parserContext.wordIt->lineNumber);
		return nullptr;
	}
	parserContext.identifiersList[varDefinitionStmt->idIndex]->type = parserContext.wordIt->lexeme;

	parserContext.wordIt++;
	if (parserContext.wordIt->tag != Tag::ASSIGN) {
		fmt::printf("Couldn't parse var definition at line %d: expected an assignment \n", parserContext.wordIt->lineNumber);
		return nullptr;
	}

	parserContext.wordIt++;
	auto initExpression = parseExpression(parserContext);

	if (initExpression == nullptr) {
		fmt::printf("Couldn't parse var definition at line %d: expected an init expression \n", parserContext.wordIt->lineNumber);
		return nullptr;
	}

	varDefinitionStmt->assignExpression = std::move(initExpression);

	return varDefinitionStmt;
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
			case Tag::MOD:
			case Tag::NE:
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
		case Tag::PLUS:
		case Tag::MINUS:
		case Tag::MUL:
		case Tag::DIV:
		case Tag::MOD:
		case Tag::NE:
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

std::shared_ptr<Expression> parseAssignmentExpression(ParserContext& parserContext) noexcept {
	fmt::printf("parsing an assignment expression\n");

	auto assignExpr = std::make_shared<AssignmentExpression>();

	for (size_t i = 0; i < parserContext.identifiersList.size(); i++) {
		if (parserContext.identifiersList[i]->name == parserContext.wordIt->lexeme) {
			assignExpr->lvalueIdIndex = i;
			break;
		}
	}
	parserContext.wordIt++;

	if (parserContext.wordIt->tag != Tag::ASSIGN) {
		fmt::printf("Couldn't parse an assignment expression at %d, %d: expected an assignment \n",
			parserContext.wordIt->lineNumber, parserContext.wordIt->posInLine);
		return nullptr;
	}
	parserContext.wordIt++;

	auto parsedExpr = parseExpression(parserContext);

	if (parsedExpr == nullptr) {
		fmt::printf("Couldn't parse an assignment expression at %d, %d: expected an expression to assign \n",
		            parserContext.wordIt->lineNumber, parserContext.wordIt->posInLine);
	}

	assignExpr->exprToAssign = parsedExpr;

	return assignExpr;
}

std::shared_ptr<Entity> parseIfStatement(ParserContext& parserContext) noexcept {
	fmt::printf("parsing an if statement\n");

	auto ifElseStmt = std::make_shared<IfElseStatement>();

	parserContext.wordIt++;
	if (parserContext.wordIt->tag != Tag::LPARENTH) {
		fmt::printf("Couldn't parse an if statement at %d, %d: expected '('.\n",
			parserContext.wordIt->lineNumber, parserContext.wordIt->posInLine);
		return nullptr;
	}

	parserContext.wordIt++;
	auto condExpr = parseExpression(parserContext);

	if (condExpr == nullptr) {
		fmt::printf("Couldn't parse an if statement at %d, %d: error parsing a condition expression.\n",
		            parserContext.wordIt->lineNumber, parserContext.wordIt->posInLine);
		return nullptr;
	}

	ifElseStmt->conditionExpression = condExpr;

	parserContext.wordIt++;
	if (parserContext.wordIt->tag != Tag::RPARENTH) {
		fmt::printf("Couldn't parse an if statement at %d, %d: expected ')'.\n",
		            parserContext.wordIt->lineNumber, parserContext.wordIt->posInLine);
		return nullptr;
	}

	parserContext.wordIt++;
	if (parserContext.wordIt->tag != Tag::THEN) {
		fmt::printf("Couldn't parse an if statement at %d, %d: expected 'then'.\n",
		            parserContext.wordIt->lineNumber, parserContext.wordIt->posInLine);
		return nullptr;
	}

	parserContext.wordIt++;
	std::vector<std::shared_ptr<Entity>> entities;
	fmt::printf("parsing the 'then' block\n");
	if (parserContext.wordIt->tag == Tag::ID) {
		fmt::printf("an id after 'then' %s\n", parserContext.wordIt->lexeme);
	}

	while (parserContext.wordIt->tag != Tag::END && parserContext.wordIt->tag != Tag::ELSE) {
		ifElseStmt->thenEntities.push_back(parseEntity(parserContext));
		parserContext.wordIt++;
	}

	fmt::printf("parsing the 'else' block\n");
	if (parserContext.wordIt->tag == Tag::ELSE) {
		parserContext.wordIt++;

		while (parserContext.wordIt->tag != Tag::END) {
			ifElseStmt->elseEntities.push_back(parseEntity(parserContext));
			parserContext.wordIt++;
		}
	}

//	if (parserContext.wordIt->tag == Tag::END) {
//		parserContext.wordIt++;
//	}

	// what if there's no end?

	return ifElseStmt;
}

std::shared_ptr<Entity> parseWhileStatement(ParserContext& parserContext) noexcept {
	fmt::printf("parsing a while statement\n");

	auto whileStmt = std::make_shared<WhileStatement>();

	parserContext.wordIt++;
	if (parserContext.wordIt->tag != Tag::LPARENTH) {
		fmt::printf("Couldn't parse a while statement at %d, %d: expected '('.\n",
		            parserContext.wordIt->lineNumber, parserContext.wordIt->posInLine);
		return nullptr;
	}

	parserContext.wordIt++;
	auto condExpr = parseExpression(parserContext);

	if (condExpr == nullptr) {
		fmt::printf("Couldn't parse a while statement at %d, %d: error parsing a condition expression.\n",
		            parserContext.wordIt->lineNumber, parserContext.wordIt->posInLine);
		return nullptr;
	}

	whileStmt->conditionExpression = condExpr;

	parserContext.wordIt++;
	if (parserContext.wordIt->tag != Tag::RPARENTH) {
		fmt::printf("Couldn't parse a while statement at %d, %d: expected ')'.\n",
		            parserContext.wordIt->lineNumber, parserContext.wordIt->posInLine);
		return nullptr;
	}

	parserContext.wordIt++;
	if (parserContext.wordIt->tag != Tag::DO) {
		fmt::printf("Couldn't parse a while statement at %d, %d: expected 'do' token.\n",
		            parserContext.wordIt->lineNumber, parserContext.wordIt->posInLine);
		return nullptr;
	}

	parserContext.wordIt++;
	std::vector<std::shared_ptr<Entity>> entities;
	fmt::printf("parsing the 'do' block\n");

	while (parserContext.wordIt->tag != Tag::END) {
		whileStmt->entities.push_back(parseEntity(parserContext));
		parserContext.wordIt++;
	}

	return whileStmt;
}

std::shared_ptr<Entity> parseNestedBlockStatement(ParserContext& parserContext) noexcept {
	fmt::printf("Parsing a nested block statement\n");

	auto stmt = std::make_shared<NestedBlockStatement>();
	parserContext.wordIt++;

	while (parserContext.wordIt->tag != Tag::RBRACE) {
		stmt->entities.push_back(parseEntity(parserContext));
		parserContext.wordIt++;
	}

	return stmt;
}

std::shared_ptr<Entity> parseReadStatement(ParserContext& parserContext) noexcept {
	fmt::printf("parsing a read statement\n");

	auto stmt = std::make_shared<ReadStatement>();
	parserContext.wordIt++;

	stmt->operand = parseExpression(parserContext);

	return stmt;
}

std::shared_ptr<Entity> parseWriteStatement(ParserContext& parserContext) noexcept {
	fmt::printf("parsing a write statement\n");

	auto stmt = std::make_shared<WriteStatement>();
	parserContext.wordIt++;

	stmt->operand = parseExpression(parserContext);

	return stmt;
}

}
