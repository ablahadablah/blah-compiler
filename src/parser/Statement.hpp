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

class ValDefinitionStatement : public Statement {
public:
	std::string name;
	std::string type;
	size_t idIndex;
	std::shared_ptr<Expression> assignExpression;
};

}

#endif //BLAHCOMPILER_STATEMENT_HPP
