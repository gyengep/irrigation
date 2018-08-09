#include <list>
#include <string>
#include "XmlReaderWriterTest.h"

using namespace std;
using namespace testing;


string remove_xml_tag(const string& text) {
	string copy(text);

	size_t pos = copy.find('>');
	if (string::npos == pos) {
		throw logic_error("check_xml");
	}

	copy.erase(0, pos + 1);
	return copy;
}
