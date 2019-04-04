#include <utility>

//
// Created by user on 4/4/19.
//

#ifndef BLAHCOMPILER_EXPRESSION_HPP
#define BLAHCOMPILER_EXPRESSION_HPP

#include <cstdint>
#include <string>

namespace blahpiler {

class Expression {
public:
	virtual ~Expression() = default;
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

class StringLiteralExpression : public Expression {
	std::string value;

public:
	explicit StringLiteralExpression(std::string newValue)
		: value(std::move(newValue)) {}
	~StringLiteralExpression() override = default;
};

}


#endif //BLAHCOMPILER_EXPRESSION_HPP
