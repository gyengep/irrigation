#include "XmlErrorWriter.h"
#include <iostream>
#include <sstream>

using namespace std;


string XmlErrorWriter::contentType() const {
	return "application/xml";
}

string XmlErrorWriter::toString(unsigned errorCode, const std::string& errorMessage) const {
	ostringstream oss;
	oss << "<?xml version=\"1.0\"?>" << endl;
	oss << "<error>" << endl;
	oss << "\t<message>" << errorMessage << "</message>" << endl;
	oss << "\t<code>" << errorCode << "</code>" << endl;
	oss << "</error>" << endl;
	return oss.str();
}
