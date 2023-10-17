#include "TemperatureHandler.h"
#include "DS18B20Wrapper.h"
#include "OWMWrapper.h"
#include "Impl/CurrentTemperatureImpl.h"
#include "Impl/TemperatureForecastImpl.h"
#include "Impl/TemperatureHistoryImpl.h"
#include "TemperatureHistoryLogger.h"
#include "Logger/Logger.h"
#include "Utils/CsvReaderImpl.h"
#include "Utils/CsvWriterImpl.h"

#include "CsvTemperatureHistoryRepository.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

TemperatureHandler::CurrentTemperatureProperties::CurrentTemperatureProperties(const std::chrono::milliseconds& updatePeriod, const std::vector<std::chrono::milliseconds>& delayOnFailed) :
	updatePeriod(updatePeriod),
	delayOnFailed(delayOnFailed)
{
}

TemperatureHandler::TemperatureForecastProperties::TemperatureForecastProperties(const std::chrono::milliseconds& updatePeriod, const std::vector<std::chrono::milliseconds>& delayOnFailed) :
	updatePeriod(updatePeriod),
	delayOnFailed(delayOnFailed)
{
}

TemperatureHandler::TemperatureHistoryProperties::TemperatureHistoryProperties(const std::chrono::seconds& length, const std::string& fileName) :
	length(length),
	fileName(fileName)
{
}

TemperatureHandler::TemperatureHistoryLoggerProperties::TemperatureHistoryLoggerProperties(const std::chrono::milliseconds& period, const std::string& fileName) :
	period(period),
	fileName(fileName)
{
}

///////////////////////////////////////////////////////////////////////////////

TemperatureHandler::TemperatureHandler(
		const CurrentTemperatureProperties& currentTemperatureProperties,
		const TemperatureForecastProperties& temperatureForecastProperties,
		const TemperatureHistoryProperties& temperatureHistoryProperties,
		const TemperatureHistoryLoggerProperties& temperatureHistoryLoggerProperties
	)
{
	current = make_shared<CurrentTemperatureImpl>(
			createCurrentTemperatureProvider()
		);

	forecast = make_shared<TemperatureForecastImpl>(
			make_shared<OWMWrapper>()
		);

	historyRepository = std::make_shared<CsvTemperatureHistoryRepository>(
			make_shared<CsvReaderFactoryImpl>(temperatureHistoryProperties.fileName),
			make_shared<CsvWriterFactoryImpl>(temperatureHistoryProperties.fileName, false)
		);

	history = make_shared<TemperatureHistoryImpl>(
			current,
			historyRepository,
			temperatureHistoryProperties.length
		);

	historyLogger = make_shared<TemperatureHistoryLogger>(
			history,
			make_shared<CsvWriterFactoryImpl>(temperatureHistoryLoggerProperties.fileName, true)
		);

	current->start(currentTemperatureProperties.updatePeriod, currentTemperatureProperties.delayOnFailed);
	forecast->start(temperatureForecastProperties.updatePeriod, temperatureForecastProperties.delayOnFailed);
	historyRepository->load();
	history->registerToListener();
	historyLogger->start();
}

TemperatureHandler::~TemperatureHandler() {
	historyLogger->stop();
	history->unregisterFromListener();
	historyRepository->save();
	forecast->stop();
	current->stop();

	historyLogger.reset();
	history.reset();
	historyRepository.reset();
	forecast.reset();
	current.reset();
}


const shared_ptr<CurrentTemperature> TemperatureHandler::getCurrentTemperature() const {
	return current;
}

const shared_ptr<TemperatureHistory> TemperatureHandler::getTemperatureHistory() const {
	return history;
}

const shared_ptr<TemperatureForecast> TemperatureHandler::getTemperatureForecast() const {
	return forecast;
}

shared_ptr<CurrentTemperatureProvider> TemperatureHandler::createCurrentTemperatureProvider() {

	try {
		return make_shared<DS18B20Wrapper>();
	} catch (const exception& e) {
		LOGGER.warning("Can not initialize DS18B20 temperature sensor", e);
	}

	return make_shared<OWMWrapper>();
}
