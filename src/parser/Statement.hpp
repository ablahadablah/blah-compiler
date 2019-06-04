//
// Created by user on 4/7/19.
//

#ifndef BLAHCOMPILER_STATEMENT_HPP
#define BLAHCOMPILER_STATEMENT_HPP

#include <string>
#include <memory>

#include "Entity.hpp"
#include "Expression.hpp"

namespace blahpiler {

class Statement : public Entity {
public:
	~Statement() override = default;
};

struct ValDefinitionStatement final : public Statement {
public:
	std::string name;
	std::string type;
	size_t idIndex;
	std::shared_ptr<Expression> assignExpression;
};

struct VarDefinitionStatement final : public Statement {
	std::string name;
	size_t idIndex;
	std::shared_ptr<Expression> assignExpression;
};

struct IfElseStatement final : public Statement {
public:
	std::shared_ptr<Expression> conditionExpression;
	std::vector<std::shared_ptr<Entity>> thenEntities;
	std::vector<std::shared_ptr<Entity>> elseEntities;

	~IfElseStatement() override = default;
};

struct WhileStatement final : public Statement {
public:
	std::shared_ptr<Expression> conditionExpression;
	std::vector<std::shared_ptr<Entity>> entities;

	~WhileStatement() override = default;
};

struct NestedBlockStatement final : public Statement {
	std::vector<std::shared_ptr<Entity>> entities;

	~NestedBlockStatement() override = default;
};

struct ReadStatement final : public Statement {
	std::shared_ptr<Expression> operand;

	~ReadStatement() override = default;
};

struct WriteStatement final : public Statement {
	std::shared_ptr<Expression> operand;

	~WriteStatement() override = default;
};

}

#endif //BLAHCOMPILER_STATEMENT_HPP
