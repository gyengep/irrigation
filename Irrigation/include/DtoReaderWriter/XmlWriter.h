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

	void saveDocument(pugi::xml_node* parent, const DocumentDto& document);
	void saveProgram(pugi::xml_node* parent, const ProgramDto& program, bool includeContainers);
	void saveRunTime(pugi::xml_node* parent, const RunTimeDto& runTime);
	void saveStartTime(pugi::xml_node* parent, const StartTimeDto& startTime);
	void saveEveryDayScheduler(pugi::xml_node* parent, const EveryDaySchedulerDto& scheduler);
	void saveHotWeatherScheduler(pugi::xml_node* parent, const HotWeatherSchedulerDto& scheduler);
	void saveTemperatureDependentScheduler(pugi::xml_node* parent, const TemperatureDependentSchedulerDto& scheduler);
	void saveWeeklyScheduler(pugi::xml_node* parent, const WeeklySchedulerDto& scheduler);
	void saveProgramList(pugi::xml_node* parent, const std::list<ProgramDto>& programs, bool includeContainers);
	void saveRunTimeList(pugi::xml_node* parent, const std::list<RunTimeDto>& runTimes);
	void saveStartTimeList(pugi::xml_node* parent, const std::list<StartTimeDto>& startTimes);
	void saveSchedulers(pugi::xml_node* parent, const SchedulersDto& schedulers);
	void saveCurrentTemperature(pugi::xml_node* parent, const CurrentTemperatureDto& currentTemperature);
	void saveTemperatureForecast(pugi::xml_node* parent, const TemperatureForecastDto& temperatureForecast);
	void saveTemperatureHistory(pugi::xml_node* parent, const TemperatureHistoryDto& temperatureHistory);

public:
	XmlWriter(bool humanReadable = true);
	virtual ~XmlWriter() = default;

	virtual std::string getContentType() const override;

	virtual std::string save(const DocumentDto& document) override;
	virtual std::string save(const ProgramDto& program) override;
	virtual std::string save(const RunTimeDto& runTime) override;
	virtual std::string save(const StartTimeDto& startTime) override;
	virtual std::string save(const EveryDaySchedulerDto& scheduler) override;
	virtual std::string save(const HotWeatherSchedulerDto& scheduler) override;
	virtual std::string save(const TemperatureDependentSchedulerDto& scheduler) override;
	virtual std::string save(const WeeklySchedulerDto& scheduler) override;
	virtual std::string save(const std::list<ProgramDto>& programs) override;
	virtual std::string save(const std::list<RunTimeDto>& runTimes) override;
	virtual std::string save(const std::list<StartTimeDto>& startTimes) override;
	virtual std::string save(const SchedulersDto& schedulers) override;

	virtual std::string save(const CurrentTemperatureDto& currentTemperature, const std::string& styleSheet) override;
	virtual std::string save(const TemperatureHistoryDto& temperatureHistory, const std::string& styleSheet) override;
	virtual std::string save(const TemperatureForecastDto& temperatureForecast, const std::string& styleSheet) override;
	virtual std::string save(const TemperatureHistoryDto& temperatureHistory, const TemperatureForecastDto& temperatureForecast, const std::string& styleSheet) override;

	virtual std::string save(const ProgramDto& program, const std::string& styleSheet) override;
	virtual std::string save(const std::list<ProgramDto>& programs, const std::string& styleSheet) override;
	virtual std::string save(const LogEntryDtoList& logEntries, const std::string& styleSheet) override;
};
