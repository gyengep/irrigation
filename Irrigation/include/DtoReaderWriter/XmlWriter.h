#pragma once
#include "DtoReaderWriter.h"


namespace pugi {
	class xml_document;
	class xml_node;
};


class XmlWriter : public DtoWriter {
protected:
	static std::string toString(const pugi::xml_document* doc, bool humanReadable);
	static std::unique_ptr<pugi::xml_document> createXmlDocument();
	static std::unique_ptr<pugi::xml_document> createXmlDocument(const std::string& styleSheet);

	const bool humanReadable;

	void saveDocumentDto(pugi::xml_node* parent, const DocumentDto& documentDto);
	void saveProgramDto(pugi::xml_node* parent, const ProgramDto& programDto, bool includeContainers);
	void saveRunTimeDto(pugi::xml_node* parent, const RunTimeDto& runTimeDto);
	void saveStartTimeDto(pugi::xml_node* parent, const StartTimeDto& startTimeDto);
	void saveEveryDaySchedulerDto(pugi::xml_node* parent, const EveryDaySchedulerDto& scheduler);
	void saveHotWeatherSchedulerDto(pugi::xml_node* parent, const HotWeatherSchedulerDto& scheduler);
	void saveTemperatureDependentSchedulerDto(pugi::xml_node* parent, const TemperatureDependentSchedulerDto& scheduler);
	void saveWeeklySchedulerDto(pugi::xml_node* parent, const WeeklySchedulerDto& scheduler);
	void saveProgramDtoList(pugi::xml_node* parent, const ProgramDtoList& programDtoList, bool includeContainers);
	void saveRunTimeDtoList(pugi::xml_node* parent, const RunTimeDtoList& runTimeDtoList);
	void saveStartTimeDtoList(pugi::xml_node* parent, const StartTimeDtoList& startTimeDtoList);
	void saveSchedulersDto(pugi::xml_node* parent, const SchedulersDto& schedulersDto);
	void saveCurrentTemperatureDto(pugi::xml_node* parent, const CurrentTemperatureDto& currentTemperatureDto);
	void saveTemperatureForecastDto(pugi::xml_node* parent, const TemperatureForecastDto& temperatureForecastDto);
	void saveTemperatureHistoryDto(pugi::xml_node* parent, const TemperatureHistoryDto& temperatureHistoryDto);

public:
	XmlWriter(bool humanReadable = true);
	virtual ~XmlWriter() = default;

	virtual std::string getContentType() const override;

	virtual std::string save(const DocumentDto& documentDto) override;
	virtual std::string save(const ProgramDto& programDto) override;
	virtual std::string save(const RunTimeDto& runTimeDto) override;
	virtual std::string save(const StartTimeDto& startTimeDto) override;
	virtual std::string save(const EveryDaySchedulerDto& schedulerDto) override;
	virtual std::string save(const HotWeatherSchedulerDto& schedulerDto) override;
	virtual std::string save(const TemperatureDependentSchedulerDto& schedulerDto) override;
	virtual std::string save(const WeeklySchedulerDto& schedulerDto) override;
	virtual std::string save(const ProgramDtoList& programDtoList) override;
	virtual std::string save(const RunTimeDtoList& runTimeDtoList) override;
	virtual std::string save(const StartTimeDtoList& startTimeDtoList) override;
	virtual std::string save(const SchedulersDto& schedulersDto) override;

	virtual std::string save(const CurrentTemperatureDto& currentTemperatureDto, const std::string& styleSheet) override;
	virtual std::string save(const TemperatureHistoryDto& temperatureHistoryDto, const std::string& styleSheet) override;
	virtual std::string save(const TemperatureForecastDto& temperatureForecastDto, const std::string& styleSheet) override;
	virtual std::string save(const TemperatureHistoryDto& temperatureHistoryDto, const TemperatureForecastDto& temperatureForecastDto, const std::string& styleSheet) override;

	virtual std::string save(const ProgramDto& programDto, const std::string& styleSheet) override;
	virtual std::string save(const ProgramDtoList& programDtoList, const std::string& styleSheet) override;
	virtual std::string save(const LogEntryDtoList& logEntryDtoList, const std::string& styleSheet) override;
};
