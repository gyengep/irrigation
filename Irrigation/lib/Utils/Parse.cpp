#include "common.h"
#include "Parse.h"


IdType Parser::parseProgramId(const std::string& text) {
	unsigned result = parse<InvalidProgramIdException>(text);
	return static_cast<IdType>(result);
}

IdType Parser::parseStartTimeId(const std::string& text) {
	unsigned result = parse<InvalidStartTimeIdException>(text);
	return static_cast<IdType>(result);
}

IdType Parser::parseRunTimeId(const std::string& text) {
	unsigned result = parse<InvalidRunTimeIdException>(text);
	return static_cast<IdType>(result);
}

IdType Parser::parseValveId(const std::string& text) {
	unsigned result = parse<InvalidValveIdException>(text);
	return static_cast<IdType>(result);
}

IdType Parser::parseZoneId(const std::string& text) {
	unsigned result = parse<InvalidZoneIdException>(text);
	return static_cast<IdType>(result);
}

unsigned Parser::parseUnsigned(const std::string& text) {
	return parse<InvalidParameterException>(text);
}

bool Parser::parseOnOff(const std::string& text) {
	bool result;

	if (text == "on") {
		result = true;
	} else if (text == "off") {
		result = false;
	} else {
		throw InvalidParameterException();
	}

	return result;
}
