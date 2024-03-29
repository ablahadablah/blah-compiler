#include <catch2/catch.hpp>

#include "lexer.hpp"

using namespace blahpiler;

TEST_CASE("Lexer", "[IntLiterals]") {
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

		REQUIRE(blahpiler::parseProgram("blah123").tokens.size() == 1);
		REQUIRE(blahpiler::parseProgram("blah:").tokens.size() == 2);
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

		REQUIRE(blahpiler::parseProgram("[").tokens.front().tag == blahpiler::Tag::LBRACKET);
		REQUIRE(blahpiler::parseProgram("]").tokens.front().tag == blahpiler::Tag::RBRACKET);
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

	SECTION("arrays") {
		using namespace blahpiler;
		std::vector<blahpiler::Token> arr{
			Token{0, 0, "arr", Tag::ID},
			Token{0, 3, "[", Tag::LBRACKET},
			Token{0, 4, "ind", Tag::ID},
			Token{0, 8, "+", Tag::PLUS},
			Token{0, 10, "1", Tag::INT},
			Token{0, 11, "]", Tag::RBRACKET}};

		REQUIRE(blahpiler::parseProgram("arr[]").tokens.size() == 3);
		REQUIRE(blahpiler::parseProgram("arr[3]").tokens.size() == 4);
		REQUIRE(blahpiler::parseProgram("arr[ind]").tokens.size() == 4);
		REQUIRE(blahpiler::parseProgram("arr[ind + 1]").tokens.size() == 6);
		REQUIRE_THAT(blahpiler::parseProgram("arr[ind + 1]").tokens, Catch::Matchers::Equals(arr));
		REQUIRE(blahpiler::parseProgram("val arr: int[1] = {1}").tokens.size() == 11);
	}
}
