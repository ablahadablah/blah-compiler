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

std::string translate(std::vector<std::shared_ptr<Entity>> const& entities, std::vector<std::shared_ptr<Identifier>>& identifiers) noexcept;

std::string translateValDefinitionStmt(ValDefinitionStatement const& valDefinitionStmt,
	std::vector<std::shared_ptr<Identifier>>& identifiers) noexcept;

std::string translateVarDefinitionStmt(VarDefinitionStatement const& valDefinitionStmt,
                                       std::vector<std::shared_ptr<Identifier>>& identifiers) noexcept;

std::string translateExpression(Expression* expr, std::vector<std::shared_ptr<Identifier>>& identifiers) noexcept;

std::string translateBinaryOperator(BinaryExpression const* expr) noexcept;

std::string translateAssignmentExpression(AssignmentExpression const* expr, std::vector<std::shared_ptr<Identifier>>& identifiers) noexcept;

std::string translateIfElseStatement(IfElseStatement const* ifElseStatement,
                                       std::vector<std::shared_ptr<Identifier>>& identifiers) noexcept;

std::string translateWhileStatement(WhileStatement const* whileStatement,
                                    std::vector<std::shared_ptr<Identifier>>& identifiers) noexcept;

std::string translateNestedBlockStatement(NestedBlockStatement const* stmt,
                                    std::vector<std::shared_ptr<Identifier>>& identifiers) noexcept;

}


#endif //BLAHCOMPILER_TRANSLATOR_HPP
