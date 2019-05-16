#include <utility>

//
// Created by user on 4/4/19.
//

#ifndef BLAHCOMPILER_EXPRESSION_HPP
#define BLAHCOMPILER_EXPRESSION_HPP

#include <cstdint>
#include <string>
#include <memory>

#include <lexer.hpp>
#include "Entity.hpp"

namespace blahpiler {

class Expression : public Entity {
public:
	~Expression() override = default;
};

//class LiteralExpression : Expression {
//public:
//	~LiteralExpression() override = default;
//};

class IntLiteralExpression : public Expression {
public:
	int32_t value;

	explicit IntLiteralExpression(int32_t newValue)
		: value(newValue) {}
	~IntLiteralExpression() override = default;
};

struct DoubleLiteralExpression : public Expression {
	double value;

	explicit DoubleLiteralExpression(double newValue)
		: value(newValue) {}
	~DoubleLiteralExpression() override = default;
};

class StringLiteralExpression : public Expression {
	std::string value;

public:
	explicit StringLiteralExpression(std::string newValue)
		: value(std::move(newValue)) {}
	~StringLiteralExpression() override = default;
};

class BinaryExpression : public Expression {
public:
	std::shared_ptr<Expression> lhs;
	std::shared_ptr<Expression> rhs;
	Tag operatorTag;

	~BinaryExpression() override = default;
};

struct IdExpression final : public Expression {
	size_t idListIndex = 0;
	std::string name;

	~IdExpression() override = default;
};

class AssignmentExpression final : public Expression {
public:
	size_t lvalueIdIndex;
	std::string lvalueName;
	std::shared_ptr<Expression> exprToAssign;

	~AssignmentExpression() override = default;
};

struct ArrayInitExpression final : public Expression {
	std::vector<std::shared_ptr<Expression>> literalExprs;

	~ArrayInitExpression() override = default;
};

}


#endif //BLAHCOMPILER_EXPRESSION_HPP
