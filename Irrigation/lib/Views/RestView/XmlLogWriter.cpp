#include "XmlLogWriter.h"
#include "Logger/Logger.h"
#include "pugixml.hpp"
#include <sstream>

using namespace std;
using namespace pugi;


string XmlLogWriter::contentType() const {
	return "application/xml";
}

string XmlLogWriter::toString(const deque<Logger::Entry>& logEntries) const {
	xml_document doc;

	xml_node processingInstructionNode = doc.prepend_child(node_pi);

	processingInstructionNode.set_name("xml-stylesheet");
	processingInstructionNode.set_value("type=\"text/xsl\" href=\"/logs.xsl\"");

	xml_node logEntriesListNode = doc.append_child("log_entries");

	for (auto entry : logEntries) {
		xml_node logEntryNode = logEntriesListNode.append_child("log_entry");

		logEntryNode.append_child("time").text().set(entry.time.c_str());
		logEntryNode.append_child("thread").text().set(entry.thread.c_str());
		logEntryNode.append_child("level").text().set(entry.level.c_str());
		logEntryNode.append_child("text").text().set(entry.text.c_str());
	}

	ostringstream oss;
	doc.save(oss);
	return oss.str();
}
