#include <utility>

//
// Created by user on 4/4/19.
//

#ifndef BLAHCOMPILER_EXPRESSION_HPP
#define BLAHCOMPILER_EXPRESSION_HPP

#include <cstdint>
#include <string>

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
	int32_t value;

public:
	explicit IntLiteralExpression(int32_t newValue)
		: value(newValue) {}
	~IntLiteralExpression() override = default;
};

class DoubleLiteralExpression : public Expression {
	double value;

public:
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
	~BinaryExpression() = default;
};

}


#endif //BLAHCOMPILER_EXPRESSION_HPP
