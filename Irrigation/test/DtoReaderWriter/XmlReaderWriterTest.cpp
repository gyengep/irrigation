#include <string>
#include "XmlReaderWriterTest.h"

using namespace testing;


std::string remove_xml_tag(const std::string& text) {
	std::string copy(text);

	size_t pos = copy.find('>');
	if (std::string::npos == pos) {
		throw std::logic_error("check_xml");
	}

	copy.erase(0, pos + 1);
	return copy;
}
