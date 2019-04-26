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
protected:
	static void loadFromString(pugi::xml_document* doc, const std::string& text);

	DocumentDTO loadDocument(const pugi::xml_node& node) const;
	ProgramDTO loadProgram(const pugi::xml_node& node) const;
	RunTimeDTO loadRunTime(const pugi::xml_node& node) const;
	StartTimeDTO loadStartTime(const pugi::xml_node& node) const;
	PeriodicSchedulerDTO loadPeriodicScheduler(const pugi::xml_node& node) const;
	WeeklySchedulerDTO loadWeeklyScheduler(const pugi::xml_node& node) const;
	std::list<ProgramDTO> loadProgramList(const pugi::xml_node& node) const;
	std::list<RunTimeDTO> loadRunTimeList(const pugi::xml_node& node) const;
	std::list<StartTimeDTO> loadStartTimeList(const pugi::xml_node& node) const;

public:
	virtual DocumentDTO loadDocument(const std::string& text) const override;
	virtual ProgramDTO loadProgram(const std::string& text) const override;
	virtual RunTimeDTO loadRunTime(const std::string& text) const override;
	virtual StartTimeDTO loadStartTime(const std::string& text) const override;
	virtual PeriodicSchedulerDTO loadPeriodicScheduler(const std::string& text) const override;
	virtual WeeklySchedulerDTO loadWeeklyScheduler(const std::string& text) const override;
	virtual std::list<ProgramDTO> loadProgramList(const std::string& text) const override;
	virtual std::list<RunTimeDTO> loadRunTimeList(const std::string& text) const override;
	virtual std::list<StartTimeDTO> loadStartTimeList(const std::string& text) const override;
};
