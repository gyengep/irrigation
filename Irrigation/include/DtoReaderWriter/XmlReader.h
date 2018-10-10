#pragma once
#include <string>
#include <memory>
#include "DtoReaderWriterFactory.h"
#include "XMLParseException.h"


namespace pugi {
	class xml_document;
	class xml_node;
};



class XmlReader : public DtoReader {
	static void loadFromString(pugi::xml_document* doc, const std::string& text);

	void loadDocument(const pugi::xml_node& node, DocumentDTO& document) const;
	void loadProgram(const pugi::xml_node& node, ProgramDTO& program) const;
	void loadRunTime(const pugi::xml_node& node, RunTimeDTO& runTime) const;
	void loadStartTime(const pugi::xml_node& node, StartTimeDTO& startTime) const;
	void loadScheduler(const pugi::xml_node& node, SpecifiedSchedulerDTO& scheduler) const;

public:
	XmlReader();
	virtual ~XmlReader();

	virtual DocumentDTO loadDocument(const std::string& text) const override;
	virtual ProgramDTO loadProgram(const std::string& text) const override;
	virtual RunTimeDTO loadRunTime(const std::string& text) const override;
	virtual StartTimeDTO loadStartTime(const std::string& text) const override;
	virtual SpecifiedSchedulerDTO loadSpecifiedScheduler(const std::string& text) const override;
};
