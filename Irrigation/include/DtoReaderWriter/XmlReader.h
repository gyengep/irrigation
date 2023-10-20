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

	DocumentDto loadDocumentDto(const pugi::xml_node& node) const;
	ProgramDto loadProgramDto(const pugi::xml_node& node) const;
	RunTimeDto loadRunTimeDto(const pugi::xml_node& node) const;
	StartTimeDto loadStartTimeDto(const pugi::xml_node& node) const;
	EveryDaySchedulerDto loadEveryDaySchedulerDto(const pugi::xml_node& node) const;
	HotWeatherSchedulerDto loadHotWeatherSchedulerDto(const pugi::xml_node& node) const;
	TemperatureDependentSchedulerDto loadTemperatureDependentSchedulerDto(const pugi::xml_node& node) const;
	WeeklySchedulerDto loadWeeklySchedulerDto(const pugi::xml_node& node) const;
	ProgramDtoList loadProgramDtoList(const pugi::xml_node& node) const;
	RunTimeDtoList loadRunTimeDtoList(const pugi::xml_node& node) const;
	StartTimeDtoList loadStartTimeDtoList(const pugi::xml_node& node) const;
	SchedulersDto loadSchedulersDto(const pugi::xml_node& node) const;

public:
	virtual DocumentDto loadDocumentDto(const std::string& text) const override;
	virtual ProgramDto loadProgramDto(const std::string& text) const override;
	virtual RunTimeDto loadRunTimeDto(const std::string& text) const override;
	virtual StartTimeDto loadStartTimeDto(const std::string& text) const override;
	virtual EveryDaySchedulerDto loadEveryDaySchedulerDto(const std::string& text) const override;
	virtual HotWeatherSchedulerDto loadHotWeatherSchedulerDto(const std::string& text) const override;
	virtual TemperatureDependentSchedulerDto loadTemperatureDependentSchedulerDto(const std::string& text) const override;
	virtual WeeklySchedulerDto loadWeeklySchedulerDto(const std::string& text) const override;
	virtual ProgramDtoList loadProgramDtoList(const std::string& text) const override;
	virtual RunTimeDtoList loadRunTimeDtoList(const std::string& text) const override;
	virtual StartTimeDtoList loadStartTimeDtoList(const std::string& text) const override;
	virtual SchedulersDto loadSchedulersDto(const std::string& text) const override;
};
