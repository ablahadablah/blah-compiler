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
		} else {
			fmt::printf("Couldn't parse an entity to anything\n");
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
	std::string translatedCode;

	if (auto binaryExpr = dynamic_cast<BinaryExpression*>(expr)) {
		translatedCode += translateExpression(binaryExpr->lhs.get(), identifiers);
		translatedCode += translateBinaryOperator(binaryExpr);
		translatedCode += translateExpression(binaryExpr->rhs.get(), identifiers);
	} else if (auto idExpr = dynamic_cast<IdExpression*>(expr)) {
		translatedCode += identifiers[idExpr->idListIndex]->name;
	} else if (auto intExpr = dynamic_cast<IntLiteralExpression*>(expr)) {
		translatedCode += std::to_string(intExpr->value);
	}

	return translatedCode;
}

std::string translateBinaryOperator(BinaryExpression const* expr) noexcept {
	if (expr->operatorTag == Tag::PLUS) {
		return " + ";
	} else {
		fmt::printf("found an unknown binary operator\n");
		return "";
	}
}

}
