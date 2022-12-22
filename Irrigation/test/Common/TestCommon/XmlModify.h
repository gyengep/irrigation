#pragma once
#include <sstream>
#include <string>


inline std::string stripXml(const std::string& text) {
	std::ostringstream oss;
    std::istringstream f(text);
    std::string line;
    while (std::getline(f, line)) {
    	line.erase(0, line.find_first_not_of("\t"));
   		oss << line;
    }

	return oss.str();
}

inline std::string prependXmlHeader(const std::string& xml) {
	static const std::string xmlHeader("<?xml version=\"1.0\"?>");
	return xmlHeader + xml;
}

inline std::string prependXmlAndStyleSheetHeader(const std::string& xml, const std::string& xslFile) {
	const std::string styleSheetPI("<?xml-stylesheet type=\"text/xsl\" href=\"" + xslFile + "\"?>");
	return prependXmlHeader(styleSheetPI + xml);
}
