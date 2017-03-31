#pragma once

#include <climits>
#include <string>


class Parser {

	template<class T> static unsigned parse(const std::string& text) {
		std::size_t pos;
		unsigned long result;
		try {
			result = std::stoul(text, &pos, 10);
		} catch (std::invalid_argument& e) {
			throw T();
		}

		if (text.length() != pos || result > UINT_MAX) {
			throw T();
		}
		return result;
	}

public:
	static IdType parseProgramId(const std::string& text);
	static IdType parseRunTimeId(const std::string& text);
	static IdType parseStartTimeId(const std::string& text);
	static IdType parseValveId(const std::string& text);
	static IdType parseZoneId(const std::string& text);
	static unsigned parseUnsigned(const std::string& text);
	static bool parseOnOff(const std::string& text);
};
