#include "XmlErrorWriter.h"
#include <pugixml/pugixml.hpp>
#include <sstream>

using namespace std;
using namespace pugi;


string XmlErrorWriter::contentType() const {
	return "application/xml";
}

string XmlErrorWriter::toString(unsigned errorCode, const string& message, const string& details) const {
	xml_document doc;
	xml_node errorNode = doc.append_child("error");

	errorNode.append_child("code").text().set(errorCode);
	errorNode.append_child("message").text().set(message.c_str());
	errorNode.append_child("details").text().set(details.c_str());

	ostringstream oss;
	doc.save(oss);
	return oss.str();
}
