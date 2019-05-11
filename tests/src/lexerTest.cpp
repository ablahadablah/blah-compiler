#include <catch2/catch.hpp>

#include "lexer.hpp"

TEST_CASE("Lexer", "IntLiterals") {
	SECTION("IntLiteral") {
		auto tokensSeq = blahpiler::parseProgram("123");

		REQUIRE(tokensSeq.tokens.size() == 1);
		REQUIRE(tokensSeq.tokens.front().tag == blahpiler::Tag::INT);
		REQUIRE(tokensSeq.tokens.front().lexeme == "123");
	}

	SECTION("ID") {
		auto tokensSeq = blahpiler::parseProgram("blah");

		REQUIRE(tokensSeq.tokens.size() == 1);
		REQUIRE(tokensSeq.tokens.front().tag == blahpiler::Tag::ID);
		REQUIRE(tokensSeq.tokens.front().lexeme == "blah");
	}

	SECTION("binaryOperators") {
		REQUIRE(blahpiler::parseProgram("*").tokens.front().tag == blahpiler::Tag::MUL);
		REQUIRE(blahpiler::parseProgram("/").tokens.front().tag == blahpiler::Tag::DIV);
		REQUIRE(blahpiler::parseProgram("+").tokens.front().tag == blahpiler::Tag::PLUS);
		REQUIRE(blahpiler::parseProgram("-").tokens.front().tag == blahpiler::Tag::MINUS);

		REQUIRE(blahpiler::parseProgram("&&").tokens.front().tag == blahpiler::Tag::AND);
		REQUIRE(blahpiler::parseProgram("||").tokens.front().tag == blahpiler::Tag::OR);
		REQUIRE(blahpiler::parseProgram(":").tokens.front().tag == blahpiler::Tag::COLON);
		REQUIRE(blahpiler::parseProgram("%").tokens.front().tag == blahpiler::Tag::MOD);

		REQUIRE(blahpiler::parseProgram("=").tokens.front().tag == blahpiler::Tag::ASSIGN);

		REQUIRE(blahpiler::parseProgram("==").tokens.front().tag == blahpiler::Tag::EQ);
		REQUIRE(blahpiler::parseProgram("!=").tokens.front().tag == blahpiler::Tag::NE);

		REQUIRE(blahpiler::parseProgram(">=").tokens.front().tag == blahpiler::Tag::GE);
		REQUIRE(blahpiler::parseProgram(">").tokens.front().tag == blahpiler::Tag::GT);
		REQUIRE(blahpiler::parseProgram("<=").tokens.front().tag == blahpiler::Tag::LE);
		REQUIRE(blahpiler::parseProgram("<").tokens.front().tag == blahpiler::Tag::LT);

		REQUIRE(blahpiler::parseProgram("{").tokens.front().tag == blahpiler::Tag::LBRACE);
		REQUIRE(blahpiler::parseProgram("}").tokens.front().tag == blahpiler::Tag::RBRACE);
		REQUIRE(blahpiler::parseProgram("(").tokens.front().tag == blahpiler::Tag::LPARENTH);
		REQUIRE(blahpiler::parseProgram(")").tokens.front().tag == blahpiler::Tag::RPARENTH);
	}

	SECTION("keywords") {
		REQUIRE(blahpiler::parseProgram("if").tokens.front().tag == blahpiler::Tag::IF);
		REQUIRE(blahpiler::parseProgram("then").tokens.front().tag == blahpiler::Tag::THEN);
		REQUIRE(blahpiler::parseProgram("else").tokens.front().tag == blahpiler::Tag::ELSE);
		REQUIRE(blahpiler::parseProgram("end").tokens.front().tag == blahpiler::Tag::END);
		REQUIRE(blahpiler::parseProgram("while").tokens.front().tag == blahpiler::Tag::WHILE);
		REQUIRE(blahpiler::parseProgram("do").tokens.front().tag == blahpiler::Tag::DO);
		REQUIRE(blahpiler::parseProgram("true").tokens.front().tag == blahpiler::Tag::TRUE);
		REQUIRE(blahpiler::parseProgram("false").tokens.front().tag == blahpiler::Tag::FALSE);
		REQUIRE(blahpiler::parseProgram("val").tokens.front().tag == blahpiler::Tag::VAL);
		REQUIRE(blahpiler::parseProgram("var").tokens.front().tag == blahpiler::Tag::VAR);
		REQUIRE(blahpiler::parseProgram("int").tokens.front().tag == blahpiler::Tag::TYPE);
		REQUIRE(blahpiler::parseProgram("double").tokens.front().tag == blahpiler::Tag::TYPE);
		REQUIRE(blahpiler::parseProgram("char").tokens.front().tag == blahpiler::Tag::TYPE);
		REQUIRE(blahpiler::parseProgram("read").tokens.front().tag == blahpiler::Tag::READ);
		REQUIRE(blahpiler::parseProgram("write").tokens.front().tag == blahpiler::Tag::WRITE);
	}

	SECTION("comments") {
		REQUIRE(blahpiler::parseProgram("//").tokens.empty());
		REQUIRE(blahpiler::parseProgram("/**/").tokens.empty());
		std::string raw = R"(//First line
Second line
Third line)";
		REQUIRE(blahpiler::parseProgram(raw).tokens.size() == 4);
	}
}
