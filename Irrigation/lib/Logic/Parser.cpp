#include "Parser.h"
#include <cstdlib>
#include <limits>
#include <stdexcept>

using namespace std;


unsigned Parser::parseUnsigned(const string& text) {

	if (text.empty()) {
		throw invalid_argument("Invalid character during conversion from string to unsigned.");
	}

	char* end = nullptr;
	unsigned long value = strtoul(text.c_str(), &end, 10);

	if (errno == ERANGE || value > numeric_limits<unsigned>::max()) {
		throw out_of_range("Out of range error during conversion from string to unsigned.");
	}

	if ('\0' != *end) {
		throw invalid_argument("Invalid character during conversion from string to unsigned.");
	}

	return (unsigned)value;
}

bool Parser::parseTrueFalse(const string& text) {

	if (text == "true") {
		return true;
	}

	if (text == "false") {
		return false;
	}

	throw invalid_argument("Invalid string during string to boolean conversion.");
}
