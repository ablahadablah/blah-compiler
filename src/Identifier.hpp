//
// Created by user on 4/7/19.
//

#ifndef BLAHCOMPILER_IDENTIFIER_HPP
#define BLAHCOMPILER_IDENTIFIER_HPP

#include <string>

namespace blahpiler {

enum class IdentifierType {
	INT = 501,
	DOUBLE, CHAR, STRING
};

class Identifier {
public:
	std::string name;
	bool isConst;

	virtual ~Identifier() = default;
};

class IntIdentifier final : public Identifier {
public:
	int32_t value = 0;

	~IntIdentifier() override = default;
};

class DoubleIdentifier final : public Identifier {
public:
	double value = 0;

	~DoubleIdentifier() override = default;
};

class CharIdentifier final : public Identifier {
public:
	char value = 0;

	~CharIdentifier() override = default;
};

class StringIdentifier final : public Identifier {
public:
	std::string value = "";

	~StringIdentifier() override = default;
};

}

#endif //BLAHCOMPILER_IDENTIFIER_HPP
