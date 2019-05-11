//
// Created by user on 4/11/19.
//

#ifndef BLAHCOMPILER_TRANSLATOR_HPP
#define BLAHCOMPILER_TRANSLATOR_HPP

#include <memory>
#include <vector>


#include <parser/parser.hpp>
#include "parser/Entity.hpp"
#include "parser/Statement.hpp"
#include "parser/Expression.hpp"

namespace blahpiler::translator {

//std::string translate(std::vector<std::shared_ptr<Entity>> const& entities, EntitiesSeq& identifiers) noexcept;

std::string translate(EntitiesSeq& entitiesSeq) noexcept;

std::string translateValDefinitionStmt(ValDefinitionStatement const& valDefinitionStmt,
	EntitiesSeq& identifiers) noexcept;

std::string translateVarDefinitionStmt(VarDefinitionStatement const& valDefinitionStmt,
                                       EntitiesSeq& identifiers) noexcept;

std::string translateExpression(Expression* expr, EntitiesSeq& identifiers) noexcept;

std::string translateBinaryOperator(BinaryExpression const* expr) noexcept;

std::string translateAssignmentExpression(AssignmentExpression const* expr, EntitiesSeq& identifiers) noexcept;

std::string translateIfElseStatement(IfElseStatement const* ifElseStatement,
                                       EntitiesSeq& identifiers) noexcept;

std::string translateWhileStatement(WhileStatement const* whileStatement,
                                    EntitiesSeq& identifiers) noexcept;

std::string translateNestedBlockStatement(NestedBlockStatement const* stmt,
                                    EntitiesSeq& identifiers) noexcept;

std::string translateReadStatement(ReadStatement const* stmt,
                                          EntitiesSeq& identifiers) noexcept;

std::string translateWriteStatement(WriteStatement const* stmt,
                                          EntitiesSeq& identifiers) noexcept;

}


#endif //BLAHCOMPILER_TRANSLATOR_HPP
