#include <catch2/catch.hpp>

#include <memory>

#include "parser/parser.hpp"

using namespace blahpiler;

TEST_CASE("Parser", "[Parser]") {
	// val arr: int[1] = {1}
	IdentifiersTable table;
	table["arr"] = std::shared_ptr<Identifier>(new Identifier{"arr", "int[1]", IdentifierType::INT, true, true, 1});
	TokensSeq tokensSeq {
		{
			Token{0, 0, "val", Tag::VAL},
			Token{0, 0, "arr", Tag::ID},
			Token{0, 3, ":", Tag::COLON},
			Token{0, 0, "int", Tag::TYPE},
			Token{0, 3, "[", Tag::LBRACKET},
			Token{0, 4, "1", Tag::INT},
			Token{0, 5, "]", Tag::RBRACKET},
			Token{0, 5, "=", Tag::ASSIGN},
			Token{0, 5, "{", Tag::LBRACE},
			Token{0, 5, "1", Tag::INT},
			Token{0, 5, "}", Tag::RBRACE},
		},
		table
	};

	SECTION("comments") {
		REQUIRE(blahpiler::parse(tokensSeq).entities.size() == 1);
	}
}

