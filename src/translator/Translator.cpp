#include "Translator.hpp"

#include <memory>

#include <fmt/printf.h>

namespace blahpiler::translator {

std::string translate(std::vector<std::shared_ptr<Entity>> const& entities, std::vector<std::shared_ptr<Identifier>>& identifiers) noexcept {
	fmt::printf("translating the program\n");

	std::string translatedCode;

	for (auto const& entity : entities) {
		if (auto valDefStmt = std::dynamic_pointer_cast<ValDefinitionStatement>(entity)) {
			fmt::print("found a val definition statement\n");

			translatedCode += translateValDefinitionStmt(*valDefStmt, identifiers);
		} else if (auto varStmt = std::dynamic_pointer_cast<VarDefinitionStatement>(entity)) {
			fmt::printf("found a var definition statement\n");

			translatedCode += translateVarDefinitionStmt(*varStmt, identifiers);
		} else if (auto assExpr = std::dynamic_pointer_cast<AssignmentExpression>(entity)) {
			fmt::printf("found an assignment expression\n");

			translatedCode += translateAssignmentExpression(assExpr.get(), identifiers);
		} else if (auto ifElseStmt = std::dynamic_pointer_cast<IfElseStatement>(entity)) {
			fmt::printf("found an if-then-else expression\n");

			translatedCode += translateIfElseStatement(ifElseStmt.get(), identifiers);
		} else if (auto whileStmt = std::dynamic_pointer_cast<WhileStatement>(entity)) {
			fmt::printf("found a while statement\n");

			translatedCode += translateWhileStatement(whileStmt.get(), identifiers);
		} else if (auto blockStmt = std::dynamic_pointer_cast<NestedBlockStatement>(entity)) {
			fmt::printf("Found a nested block statement\n");
			translatedCode += translateNestedBlockStatement(blockStmt.get(), identifiers);
		} else {
			fmt::printf("Translation failure: unknown entity\n");
		}
	}

	return translatedCode;
}

std::string translateValDefinitionStmt(ValDefinitionStatement const& valDefinitionStmt,
	std::vector<std::shared_ptr<Identifier>>& identifiers) noexcept {
	std::string translatedCode;
	auto id = identifiers[valDefinitionStmt.idIndex];

	translatedCode += "const ";

	if (valDefinitionStmt.type == "int") {
		translatedCode += "int ";
	} else if (valDefinitionStmt.type == "char") {
		translatedCode += "char ";
	} else {
		fmt::printf("Couldn't translate val definition statement\n");
		return "";
	}

	translatedCode += id->name;

	if (valDefinitionStmt.assignExpression != nullptr) {
		translatedCode += " = ";

		translatedCode += translateExpression(valDefinitionStmt.assignExpression.get(), identifiers);
	}

	translatedCode += ";";

	return translatedCode;
}

std::string translateVarDefinitionStmt(VarDefinitionStatement const& varDefinitionStmt,
                                       std::vector<std::shared_ptr<Identifier>>& identifiers) noexcept {
	std::string translatedCode;
	auto id = identifiers[varDefinitionStmt.idIndex];

	if (identifiers[varDefinitionStmt.idIndex]->type == "int") {
		translatedCode += "int ";
	} else if (identifiers[varDefinitionStmt.idIndex]->type == "char") {
		translatedCode += "char ";
	} else {
		fmt::printf("Couldn't translate var definition statement\n");
		return "";
	}

	translatedCode += id->name;

	if (varDefinitionStmt.assignExpression != nullptr) {
		translatedCode += " = ";

		translatedCode += translateExpression(varDefinitionStmt.assignExpression.get(), identifiers);
	}

	translatedCode += ";";

	return translatedCode;
}

std::string translateExpression(Expression* expr, std::vector<std::shared_ptr<Identifier>>& identifiers) noexcept {
	fmt::printf("translating an expression\n");
	std::string translatedCode;

	if (auto binaryExpr = dynamic_cast<BinaryExpression*>(expr)) {
		fmt::printf("found a binary expression\n");
		translatedCode += translateExpression(binaryExpr->lhs.get(), identifiers);
		translatedCode += translateBinaryOperator(binaryExpr);
		translatedCode += translateExpression(binaryExpr->rhs.get(), identifiers);
	} else if (auto idExpr = dynamic_cast<IdExpression*>(expr)) {
		fmt::printf("found an id expression\n");
		translatedCode += identifiers[idExpr->idListIndex]->name;
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
	std::vector<std::shared_ptr<Identifier>>& identifiers) noexcept {
	std::string translatedCode;

	if (identifiers[expr->lvalueIdIndex]->isConst) {
		fmt::printf("Couldn't parse an assignment expression: lvalue is const\n");
		return "";
	} else if (expr->lvalueIdIndex >= identifiers.size()) {
		fmt::printf("Couldn't parse an assignment expression: wrong id index\n");
		return "";
	}

	translatedCode += identifiers[expr->lvalueIdIndex]->name;
	translatedCode += " = ";
	translatedCode += translateExpression(expr->exprToAssign.get(), identifiers);
	translatedCode += "; \n";

	return translatedCode;
}

std::string translateIfElseStatement(IfElseStatement const* ifElseStatement,
                                     std::vector<std::shared_ptr<Identifier>>& identifiers) noexcept {
	std::string translatedCode;

	translatedCode += "if (";
	translatedCode += translateExpression(ifElseStatement->conditionExpression.get(), identifiers);
	translatedCode += ") { ";

	translatedCode += translate(ifElseStatement->thenEntities, identifiers);

	if (!ifElseStatement->elseEntities.empty()) {
		translatedCode += "} else { ";
		translatedCode += translate(ifElseStatement->elseEntities, identifiers);
	}

	translatedCode += "} \n";

	return translatedCode;
}

std::string translateWhileStatement(WhileStatement const* whileStatement,
                                     std::vector<std::shared_ptr<Identifier>>& identifiers) noexcept {
	std::string translatedCode;

	translatedCode += "while (";
	translatedCode += translateExpression(whileStatement->conditionExpression.get(), identifiers);
	translatedCode += ") { ";

	translatedCode += translate(whileStatement->entities, identifiers);
	translatedCode += "} \n";

	return translatedCode;
}

std::string translateNestedBlockStatement(NestedBlockStatement const* stmt,
                                          std::vector<std::shared_ptr<Identifier>>& identifiers) noexcept {
	return "{ \n" + translate(stmt->entities, identifiers) + "\n } \n";
}

}
