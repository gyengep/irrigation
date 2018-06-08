#include "RunTime.h"
#include "Parser.h"
#include <stdexcept>

using namespace std;


RunTime::RunTime() {
	setValue(0);
}

RunTime::~RunTime() {
}

unsigned RunTime::getValue() const {
	return seconds;
}

void RunTime::setValue(unsigned seconds) {
	if (seconds > maxSeconds) {
		throw out_of_range("Runtime is out of range");
	}

	this->seconds = seconds;
}

/*
void RunTime::fromString(const string& text) {
	unsigned value = 0;
	try {
		value = Parser::parseUnsigned(text);
	} catch(out_of_range& e) {
		throw out_of_range("RunTime is out of range");
	} catch (invalid_argument& e) {
		throw invalid_argument("RunTime string contains invalid characters");
	}

	setValue(value);
}
*/

RunTimeDTO RunTime::getRunTimeDTO() const {
	return RunTimeDTO(getValue());
}
