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

	void saveDocument(pugi::xml_node* parent, const DocumentDTO& document);
	void saveProgram(pugi::xml_node* parent, const ProgramDTO& program, bool includeContainers);
	void saveRunTime(pugi::xml_node* parent, const RunTimeDTO& runTime);
	void saveStartTime(pugi::xml_node* parent, const StartTimeDTO& startTime);
	void saveEveryDayScheduler(pugi::xml_node* parent, const EveryDaySchedulerDTO& scheduler);
	void saveHotWeatherScheduler(pugi::xml_node* parent, const HotWeatherSchedulerDTO& scheduler);
	void saveTemperatureDependentScheduler(pugi::xml_node* parent, const TemperatureDependentSchedulerDTO& scheduler);
	void saveWeeklyScheduler(pugi::xml_node* parent, const WeeklySchedulerDTO& scheduler);
	void saveProgramList(pugi::xml_node* parent, const std::list<ProgramDTO>& programs, bool includeContainers);
	void saveRunTimeList(pugi::xml_node* parent, const std::list<RunTimeDTO>& runTimes);
	void saveStartTimeList(pugi::xml_node* parent, const std::list<StartTimeDTO>& startTimes);
	void saveSchedulers(pugi::xml_node* parent, const SchedulersDTO& schedulers);
	void saveCurrentTemperature(pugi::xml_node* parent, const CurrentTemperatureDTO& currentTemperature);
	void saveTemperatureForecast(pugi::xml_node* parent, const TemperatureForecastDTO& temperatureForecast);
	void saveTemperatureHistory(pugi::xml_node* parent, const TemperatureHistoryDTO& temperatureHistory);

public:
	XmlWriter(bool humanReadable = true);
	virtual ~XmlWriter() = default;

	virtual std::string getContentType() const override;

	virtual std::string save(const DocumentDTO& document) override;
	virtual std::string save(const ProgramDTO& program) override;
	virtual std::string save(const RunTimeDTO& runTime) override;
	virtual std::string save(const StartTimeDTO& startTime) override;
	virtual std::string save(const EveryDaySchedulerDTO& scheduler) override;
	virtual std::string save(const HotWeatherSchedulerDTO& scheduler) override;
	virtual std::string save(const TemperatureDependentSchedulerDTO& scheduler) override;
	virtual std::string save(const WeeklySchedulerDTO& scheduler) override;
	virtual std::string save(const std::list<ProgramDTO>& programs) override;
	virtual std::string save(const std::list<RunTimeDTO>& runTimes) override;
	virtual std::string save(const std::list<StartTimeDTO>& startTimes) override;
	virtual std::string save(const SchedulersDTO& schedulers) override;

	virtual std::string save(const CurrentTemperatureDTO& currentTemperature, const std::string& styleSheet) override;
	virtual std::string save(const TemperatureHistoryDTO& temperatureHistory, const std::string& styleSheet) override;
	virtual std::string save(const TemperatureForecastDTO& temperatureForecast, const std::string& styleSheet) override;
	virtual std::string save(const TemperatureHistoryDTO& temperatureHistory, const TemperatureForecastDTO& temperatureForecast, const std::string& styleSheet) override;

	virtual std::string save(const ProgramDTO& program, const std::string& styleSheet) override;
	virtual std::string save(const std::list<ProgramDTO>& programs, const std::string& styleSheet) override;
};
