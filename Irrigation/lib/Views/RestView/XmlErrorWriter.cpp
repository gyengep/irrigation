#include "XmlErrorWriter.h"
#include <iostream>
#include <sstream>

using namespace std;


string XmlErrorWriter::to(const string& errorMessage, unsigned errorCode) {
	ostringstream oss;
	oss << "<?xml version=\"1.0\"?>" << endl;
	oss << "<error>" << endl;
	oss << "\t<message>" << errorMessage << "</message>" << endl;
	oss << "\t<code>" << errorCode << "</code>" << endl;
	oss << "</error>" << endl;
	return oss.str();
}

unique_ptr<ErrorWriter> XmlErrorWriterFactory::create() const {
	return unique_ptr<ErrorWriter>(new XmlErrorWriter());
}
