#pragma once
#include <string>
#include <memory>
#include "XMLParseException.h"


class DocumentDTO;
class ProgramDTO;
class RunTimeDTO;
class StartTimeDTO;
class SpecifiedSchedulerDTO;

namespace pugi {
	class xml_document;
	class xml_node;
};



class XmlReader {
	static void loadFromString(pugi::xml_document* doc, const std::string& text);

	void loadDocument(const pugi::xml_node& node, DocumentDTO& document) const;
	void loadProgram(const pugi::xml_node& node, ProgramDTO& program) const;
	void loadRunTime(const pugi::xml_node& node, RunTimeDTO& runTime) const;
	void loadStartTime(const pugi::xml_node& node, StartTimeDTO& startTime) const;
	void loadScheduler(const pugi::xml_node& node, SpecifiedSchedulerDTO& scheduler) const;

public:
	XmlReader();
	virtual ~XmlReader();

	virtual DocumentDTO loadDocument(const std::string& text) const;
	virtual ProgramDTO loadProgram(const std::string& text) const;
	virtual RunTimeDTO loadRunTime(const std::string& text) const;
	virtual StartTimeDTO loadStartTime(const std::string& text) const;
	virtual SpecifiedSchedulerDTO loadSpecifiedScheduler(const std::string& text) const;
};
