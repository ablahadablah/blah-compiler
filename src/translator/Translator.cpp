#include "Translator.hpp"

#include <memory>

#include <fmt/printf.h>

namespace blahpiler::translator {

//std::string translate(std::vector<std::shared_ptr<Entity>> const& entities, EntitiesSeq& identifiers) noexcept {
std::string translate(EntitiesSeq& entitiesSeq) noexcept {
	fmt::printf("translating the program\n");

	std::string translatedCode;

	for (auto const& entity : entitiesSeq.entities) {
		if (auto valDefStmt = std::dynamic_pointer_cast<ValDefinitionStatement>(entity)) {
			fmt::print("found a val definition statement\n");

			translatedCode += translateValDefinitionStmt(*valDefStmt, entitiesSeq);
		} else if (auto varStmt = std::dynamic_pointer_cast<VarDefinitionStatement>(entity)) {
			fmt::printf("found a var definition statement\n");

			translatedCode += translateVarDefinitionStmt(*varStmt, entitiesSeq);
		} else if (auto assExpr = std::dynamic_pointer_cast<AssignmentExpression>(entity)) {
			fmt::printf("found an assignment expression\n");

			translatedCode += translateAssignmentExpression(assExpr.get(), entitiesSeq);
		} else if (auto ifElseStmt = std::dynamic_pointer_cast<IfElseStatement>(entity)) {
			fmt::printf("found an if-then-else expression\n");

			translatedCode += translateIfElseStatement(ifElseStmt.get(), entitiesSeq);
		} else if (auto whileStmt = std::dynamic_pointer_cast<WhileStatement>(entity)) {
			fmt::printf("found a while statement\n");

			translatedCode += translateWhileStatement(whileStmt.get(), entitiesSeq);
		} else if (auto blockStmt = std::dynamic_pointer_cast<NestedBlockStatement>(entity)) {
			fmt::printf("Found a nested block statement\n");
			translatedCode += translateNestedBlockStatement(blockStmt.get(), entitiesSeq);
		} else if (auto readStmt = std::dynamic_pointer_cast<ReadStatement>(entity)) {
			fmt::printf("found a read statement\n");
			translatedCode += translateReadStatement(readStmt.get(), entitiesSeq);
		} else if (auto writeStmt = std::dynamic_pointer_cast<WriteStatement>(entity)) {
			fmt::printf("found a write statement\n");
			translatedCode += translateWriteStatement(writeStmt.get(), entitiesSeq);
		} else {
			fmt::printf("Translation failure: unknown entity\n");
		}
	}

	return translatedCode;
}

std::string translateValDefinitionStmt(ValDefinitionStatement const& valDefinitionStmt,
	EntitiesSeq& identifiers) noexcept {
	std::string translatedCode;
	auto id = identifiers.idsTable[valDefinitionStmt.name];

	translatedCode += "const ";

	if (identifiers.idsTable[valDefinitionStmt.name]->type == IdentifierType::INT) {
		translatedCode += "int ";
	} else if (identifiers.idsTable[valDefinitionStmt.name]->type == IdentifierType::DOUBLE) {
		translatedCode += "double ";
	} else {
		fmt::printf("Couldn't translate val definition statement\n");
		return "";
	}

	translatedCode += valDefinitionStmt.name;

	if (valDefinitionStmt.assignExpression != nullptr) {
		translatedCode += " = ";

		translatedCode += translateExpression(valDefinitionStmt.assignExpression.get(), identifiers);
	}

	translatedCode += ";\n";

	return translatedCode;
}

std::string translateVarDefinitionStmt(VarDefinitionStatement const& varDefinitionStmt,
                                       EntitiesSeq& identifiers) noexcept {
	std::string translatedCode;
	auto id = identifiers.idsTable[varDefinitionStmt.name];

	if (identifiers.idsTable[varDefinitionStmt.name]->type == IdentifierType::INT) {
		translatedCode += "int ";
	} else if (identifiers.idsTable[varDefinitionStmt.name]->type == IdentifierType::DOUBLE) {
		translatedCode += "double ";
	} else {
		fmt::printf("Couldn't translate var definition statement: unknown type\n");
		return "";
	}

	translatedCode += varDefinitionStmt.name;

	if (varDefinitionStmt.assignExpression != nullptr) {
		translatedCode += " = ";

		translatedCode += translateExpression(varDefinitionStmt.assignExpression.get(), identifiers);
	}

	translatedCode += ";\n";

	return translatedCode;
}

std::string translateExpression(Expression* expr, EntitiesSeq& identifiers) noexcept {
	fmt::printf("translating an expression\n");
	std::string translatedCode;

	if (auto binaryExpr = dynamic_cast<BinaryExpression*>(expr)) {
		fmt::printf("found a binary expression\n");
		translatedCode += translateExpression(binaryExpr->lhs.get(), identifiers);
		translatedCode += translateBinaryOperator(binaryExpr);
		translatedCode += translateExpression(binaryExpr->rhs.get(), identifiers);
	} else if (auto idExpr = dynamic_cast<IdExpression*>(expr)) {
		fmt::printf("found an id expression\n");
		translatedCode += idExpr->name;
	} else if (auto intExpr = dynamic_cast<IntLiteralExpression*>(expr)) {
		fmt::printf("found an int literal expression\n");
		translatedCode += std::to_string(intExpr->value);
	} else {
		fmt::printf("couldn't translate an expression\n");
	}

	return translatedCode;
}

std::string translateBinaryOperator(BinaryExpression const* expr) noexcept {
	switch (expr->operatorTag) {
		case Tag::PLUS:
			return " + ";
		case Tag::OR:
			return " || ";
		case Tag::AND:
			return " && ";
		case Tag::ASSIGN:
			return " = ";
		case Tag::EQ:
			return " == ";
		case Tag::GE:
			return " >= ";
		case Tag::GT:
			return " > ";
		case Tag::LE:
			return " <= ";
		case Tag::LT:
			return " < ";
		case Tag::MINUS:
			return " - ";
		case Tag::MUL:
			return " * ";
		case Tag::DIV:
			return " / ";
		case Tag::MOD:
			return " % ";
		case Tag::NE:
			return " != ";
		default:
			fmt::printf("found an unknown binary operator\n");
			return "";
	}
}

std::string translateAssignmentExpression(AssignmentExpression const* expr,
	EntitiesSeq& identifiers) noexcept {
	std::string translatedCode;

	if (identifiers.idsTable[expr->lvalueName]->isConst) {
		fmt::printf("Couldn't parse an assignment expression: lvalue is const\n");
		return "";
	} else if (expr->lvalueIdIndex >= identifiers.idsTable.size()) {
		fmt::printf("Couldn't parse an assignment expression: wrong id index\n");
		return "";
	}

	translatedCode += identifiers.idsTable[expr->lvalueName]->name;
	translatedCode += " = ";
	translatedCode += translateExpression(expr->exprToAssign.get(), identifiers);
	translatedCode += "; \n";

	return translatedCode;
}

std::string translateIfElseStatement(IfElseStatement const* ifElseStatement,
                                     EntitiesSeq& identifiers) noexcept {
	std::string translatedCode;

	translatedCode += "if (";
	translatedCode += translateExpression(ifElseStatement->conditionExpression.get(), identifiers);
	translatedCode += ") { ";

	auto stmtEntitiesSeq = EntitiesSeq{ifElseStatement->thenEntities, identifiers.idsTable};
	translatedCode += translate(stmtEntitiesSeq);

	if (!ifElseStatement->elseEntities.empty()) {
		translatedCode += "} else { ";
		auto elseStmtEntitiesSeq = EntitiesSeq{ifElseStatement->thenEntities, identifiers.idsTable};
		translatedCode += translate(elseStmtEntitiesSeq);
	}

	translatedCode += "} \n";

	return translatedCode;
}

std::string translateWhileStatement(WhileStatement const* whileStatement,
                                     EntitiesSeq& identifiers) noexcept {
	std::string translatedCode;

	translatedCode += "while (";
	translatedCode += translateExpression(whileStatement->conditionExpression.get(), identifiers);
	translatedCode += ") { ";

	auto stmtEntitiesSeq = EntitiesSeq{whileStatement->entities, identifiers.idsTable};
	translatedCode += translate(stmtEntitiesSeq);
	translatedCode += "} \n";

	return translatedCode;
}

std::string translateNestedBlockStatement(NestedBlockStatement const* stmt,
                                          EntitiesSeq& identifiers) noexcept {
	auto stmtEntitiesSeq = EntitiesSeq{stmt->entities, identifiers.idsTable};
	return "{ \n" + translate(stmtEntitiesSeq) + "\n } \n";
}

std::string translateReadStatement(ReadStatement const* stmt,
                                   EntitiesSeq& identifiers) noexcept {
	std::string translatedCode;

	if (auto idExpr = std::dynamic_pointer_cast<IdExpression>(stmt->operand)) {
		fmt::printf("found an id expr in the operand: %s\n", identifiers.idsTable[idExpr->name]->name);
		fmt::printf("id expr type: %s, %d\n", identifiers.idsTable[idExpr->name]->typeLexeme,
			static_cast<int>(identifiers.idsTable[idExpr->name]->type));
		switch (identifiers.idsTable[idExpr->name]->type) {
			case IdentifierType::INT:
				translatedCode += fmt::format("scanf(\"%d\", &{}); \n", idExpr->name);
				break;
			case IdentifierType::DOUBLE:
				translatedCode += fmt::format("scanf(\"%f\", &{}); \n", idExpr->name);
				break;
			default:
				fmt::printf("Cannot translate read statement: unknown operand type\n");
		}
	} else {
		fmt::printf("Cannot translate read statement: operand should be an identifier\n");
	}

	return translatedCode;
}

std::string translateWriteStatement(WriteStatement const* stmt,
                                    EntitiesSeq& identifiers) noexcept {
	std::string translatedCode;

	if (auto idExpr = std::dynamic_pointer_cast<IdExpression>(stmt->operand)) {
		fmt::printf("found an id expr in the operand: %s\n", identifiers.idsTable[idExpr->name]->name);
		fmt::printf("id expr type: %s, %d\n", identifiers.idsTable[idExpr->name]->typeLexeme,
			static_cast<int>(identifiers.idsTable[idExpr->name]->type));
		switch (identifiers.idsTable[idExpr->name]->type) {
			case IdentifierType::INT:
				translatedCode += fmt::format("printf(\"%d\\n\", {});", idExpr->name);
				break;
			case IdentifierType::DOUBLE:
				translatedCode += fmt::format("printf(\"%f\\n\", {});", idExpr->name);
				break;
			default:
				fmt::printf("Cannot translate read statement: unknown operand type\n");
		}
	} else if (auto intExpr = std::dynamic_pointer_cast<IntLiteralExpression>(stmt->operand)) {
		translatedCode += fmt::format("printf(\"%d\\n\", {});", intExpr->value);
	} else if (auto doubleExpr = std::dynamic_pointer_cast<DoubleLiteralExpression>(stmt->operand)) {
		translatedCode += fmt::format("printf(\"%d\\n\", {});", doubleExpr->value);
	} else {
		fmt::printf("Could'nt translate write statement\n");
	}

	return translatedCode;
}

}
