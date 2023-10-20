#pragma once
#include "DtoReaderWriter.h"
#include "XMLParseException.h"


namespace pugi {
	class xml_document;
	class xml_node;
};


class XmlReader : public DtoReader {
protected:
	static void loadFromString(pugi::xml_document* doc, const std::string& text);

	DocumentDto loadDocument(const pugi::xml_node& node) const;
	ProgramDto loadProgram(const pugi::xml_node& node) const;
	RunTimeDto loadRunTime(const pugi::xml_node& node) const;
	StartTimeDto loadStartTime(const pugi::xml_node& node) const;
	EveryDaySchedulerDto loadEveryDayScheduler(const pugi::xml_node& node) const;
	HotWeatherSchedulerDto loadHotWeatherScheduler(const pugi::xml_node& node) const;
	TemperatureDependentSchedulerDto loadTemperatureDependentScheduler(const pugi::xml_node& node) const;
	WeeklySchedulerDto loadWeeklyScheduler(const pugi::xml_node& node) const;
	std::list<ProgramDto> loadProgramList(const pugi::xml_node& node) const;
	std::list<RunTimeDto> loadRunTimeList(const pugi::xml_node& node) const;
	std::list<StartTimeDto> loadStartTimeList(const pugi::xml_node& node) const;
	SchedulersDto loadSchedulers(const pugi::xml_node& node) const;

public:
	virtual DocumentDto loadDocument(const std::string& text) const override;
	virtual ProgramDto loadProgram(const std::string& text) const override;
	virtual RunTimeDto loadRunTime(const std::string& text) const override;
	virtual StartTimeDto loadStartTime(const std::string& text) const override;
	virtual EveryDaySchedulerDto loadEveryDayScheduler(const std::string& text) const override;
	virtual HotWeatherSchedulerDto loadHotWeatherScheduler(const std::string& text) const override;
	virtual TemperatureDependentSchedulerDto loadTemperatureDependentScheduler(const std::string& text) const override;
	virtual WeeklySchedulerDto loadWeeklyScheduler(const std::string& text) const override;
	virtual std::list<ProgramDto> loadProgramList(const std::string& text) const override;
	virtual std::list<RunTimeDto> loadRunTimeList(const std::string& text) const override;
	virtual std::list<StartTimeDto> loadStartTimeList(const std::string& text) const override;
	virtual SchedulersDto loadSchedulers(const std::string& text) const override;
};
