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
	std::string typeLexeme;
	IdentifierType type;
	bool isConst;
	bool arrayType = false;
	int64_t arraySize = 0;
};

}

#endif //BLAHCOMPILER_IDENTIFIER_HPP
