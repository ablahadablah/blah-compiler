//
// Created by user on 4/2/19.
//

#include "parser.hpp"

#include <memory>
#include <string>

namespace blahpiler {

void parse(std::vector<Word> const& words) noexcept {
	fmt::printf("parsing the program\n");

	auto peek = words.begin();
	std::vector<std::unique_ptr<Entity>> entities;

	do {
		entities.push_back(parseEntity(peek));

		peek += 1;
	} while(peek != words.end());

	fmt::printf("Entities num: %d\n", entities.size());
}

std::unique_ptr<BinaryExpression> parseBinaryOperator(Word const& word) noexcept {

}

std::unique_ptr<Expression> parseLiteralExpression(WordIt& wordIt) noexcept {
	fmt::printf("parsing a literal expression\n");

	switch (wordIt->tag) {
		case Tag::INT:
			return std::unique_ptr<Expression>(new IntLiteralExpression(std::stoi(wordIt->lexeme)));
		case Tag::DOUBLE:
			return std::make_unique<DoubleLiteralExpression>(std::stod(wordIt->lexeme));
		default:
			fmt::printf("Some wrong tag, cannot parse a literal expression\n");
	}
}

std::unique_ptr<Entity> parseEntity(WordIt& wordIt) noexcept {
	fmt::printf("Parsing an entity\n");

	switch (wordIt->tag) {
		case Tag::OR:
		case Tag::AND:
		case Tag::ASSIGN:
		case Tag::EQ:
		case Tag::GE:
		case Tag::GT:
		case Tag::LE:
		case Tag::LT:
			// need to somehow access the previous token
			return parseBinaryOperator(*wordIt);
		case Tag::INT:
			return parseLiteralExpression(wordIt);
		case Tag::FUN:
			return parseFunctionDefinitionStatement(wordIt);
		case Tag::VAL:
			return parseValDefinitionStatement(wordIt);
		default:
			fmt::printf("blah blah blah default: %s\n", wordIt->lexeme);
	}
}

std::unique_ptr<Entity> parseFunctionDefinitionStatement(WordIt& wordIt) noexcept {
	if ((++wordIt)->tag != Tag::ID) {
		fmt::printf("Couldn't parse function definition at line %d\n", wordIt->lineNumber);
		return nullptr;
	}
	if ((++wordIt)->tag != Tag::LPARENTH) {
		fmt::printf("Couldn't parse function definition at line %d\n", wordIt->lineNumber);
		return nullptr;
	}
	if ((++wordIt)->tag != Tag::RPARENTH) {
		fmt::printf("Couldn't parse function definition at line %d\n", wordIt->lineNumber);
		return nullptr;
	}
	if ((++wordIt)->tag != Tag::COLON) {
		fmt::printf("Couldn't parse function definition at line %d\n", wordIt->lineNumber);
		return nullptr;
	}
	if ((++wordIt)->tag != Tag::LBRACE) {
		fmt::printf("Couldn't parse function definition at line %d\n", wordIt->lineNumber);
		return nullptr;
	}
	if ((++wordIt)->tag != Tag::RBRACE) {
		fmt::printf("Couldn't parse function definition at line %d\n", wordIt->lineNumber);
		return nullptr;
	}
}

std::unique_ptr<Entity> parseValDefinitionStatement(WordIt& wordIt) noexcept {
	fmt::printf("parsing a val definition\n");
	auto valDefinitionStmt = std::make_unique<ValDefinitionStatement>();

	++wordIt;
	if ((wordIt)->tag != Tag::ID) {
		fmt::printf("Couldn't parse val definition at line %d: expected an ID\n", wordIt->lineNumber);
		return nullptr;
	}
	valDefinitionStmt->name = wordIt->lexeme;

	if ((++wordIt)->tag != Tag::COLON) {
		fmt::printf("Couldn't parse val definition at line %d: expected a colon\n", wordIt->lineNumber);
		return nullptr;
	}
	if ((++wordIt)->tag != Tag::TYPE) {
		fmt::printf("Couldn't parse val at line %d: expected a type\n", wordIt->lineNumber);
		return nullptr;
	}
	valDefinitionStmt->name = wordIt->lexeme;

	if ((++wordIt)->tag != Tag::ASSIGN) {
		fmt::printf("Couldn't parse val definition at line %d: expected an assignment \n", wordIt->lineNumber);
		return nullptr;
	}

	auto initExpression = parseExpression(++wordIt);

	if (initExpression == nullptr) {
		fmt::printf("Couldn't parse val definition at line %d: expected an init expression \n", wordIt->lineNumber);
		return nullptr;
	}

	valDefinitionStmt->assignExpression = std::move(initExpression);

	return valDefinitionStmt;
}

std::unique_ptr<Expression> parseExpression(WordIt& wordIt) noexcept {
	fmt::printf("parsing an expression\n");

	switch (wordIt->tag) {
		case Tag::OR:
		case Tag::AND:
		case Tag::ASSIGN:
		case Tag::EQ:
		case Tag::GE:
		case Tag::GT:
		case Tag::LE:
		case Tag::LT:
			// need to somehow access the previous token
			return parseBinaryOperator(*wordIt);
		case Tag::INT:
			return parseLiteralExpression(wordIt);
		default:
			fmt::printf("Couldn't parse an expression at line %d: wrong tag\n", wordIt->lineNumber);
	}
}

}
