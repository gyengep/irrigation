#pragma once
#include <string>


class Parser {
public:
	static unsigned parseUnsigned(const std::string& text);
	static bool parseTrueFalse(const std::string& text);
};
