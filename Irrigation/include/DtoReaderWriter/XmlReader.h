#pragma once
#include <string>
#include <memory>
#include "DtoReaderWriter.h"
#include "XMLParseException.h"


namespace pugi {
	class xml_document;
	class xml_node;
};


class XmlReader : public DtoReader {
	static void loadFromString(pugi::xml_document* doc, const std::string& text);

	DocumentDTO loadDocument(const pugi::xml_node& node) const;
	ProgramDTO loadProgram(const pugi::xml_node& node) const;
	RunTimeDTO loadRunTime(const pugi::xml_node& node) const;
	StartTimeDTO loadStartTime(const pugi::xml_node& node) const;
	SpecifiedSchedulerDTO loadScheduler(const pugi::xml_node& node) const;

public:
	virtual DocumentDTO loadDocument(const std::string& text) const override;
	virtual ProgramDTO loadProgram(const std::string& text) const override;
	virtual RunTimeDTO loadRunTime(const std::string& text) const override;
	virtual StartTimeDTO loadStartTime(const std::string& text) const override;
	virtual SpecifiedSchedulerDTO loadSpecifiedScheduler(const std::string& text) const override;
};
