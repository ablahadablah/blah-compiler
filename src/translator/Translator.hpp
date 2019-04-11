//
// Created by user on 4/11/19.
//

#ifndef BLAHCOMPILER_TRANSLATOR_HPP
#define BLAHCOMPILER_TRANSLATOR_HPP

#include <memory>
#include <vector>

#include "parser/Entity.hpp"
#include "parser/Statement.hpp"
#include "parser/Expression.hpp"

namespace blahpiler::translator {

std::string translate(std::vector<std::shared_ptr<Entity>> const& entities) noexcept;

std::string translateValDefinitionStmt(ValDefinitionStatement const& valDefinitionStmt) noexcept;

std::string translateExpression(Expression* expr) noexcept;

std::string translateBinaryOperator(BinaryExpression const* expr) noexcept;

}


#endif //BLAHCOMPILER_TRANSLATOR_HPP
