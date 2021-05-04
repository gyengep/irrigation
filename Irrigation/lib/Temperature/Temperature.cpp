#include "Temperature.h"
#include "DS18B20Wrapper.h"
#include "DarkSkyWrapper.h"
#include "CurrentTemperatureImpl.h"
#include "TemperatureForecastImpl.h"
#include "TemperatureHistoryImpl.h"
#include "TemperatureHistoryLogger.h"
#include "Logger/Logger.h"
#include "Utils/CsvReaderImpl.h"
#include "Utils/CsvWriterImpl.h"

#include "CsvTemperatureHistoryPersister.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

Temperature::CurrentTemperatureProperties::CurrentTemperatureProperties(const std::chrono::milliseconds& updatePeriod, const std::vector<std::chrono::milliseconds>& delayOnFailed) :
	updatePeriod(updatePeriod),
	delayOnFailed(delayOnFailed)
{
}

Temperature::TemperatureForecastProperties::TemperatureForecastProperties(const std::chrono::milliseconds& updatePeriod, const std::vector<std::chrono::milliseconds>& delayOnFailed) :
	updatePeriod(updatePeriod),
	delayOnFailed(delayOnFailed)
{
}

Temperature::TemperatureHistoryProperties::TemperatureHistoryProperties(const std::chrono::seconds& length, const std::string& fileName) :
	length(length),
	fileName(fileName)
{
}

Temperature::TemperatureHistoryLoggerProperties::TemperatureHistoryLoggerProperties(const std::chrono::milliseconds& period, const std::string& fileName) :
	period(period),
	fileName(fileName)
{
}

///////////////////////////////////////////////////////////////////////////////

Temperature& Temperature::getInstance() {
	static Temperature instance;
	return instance;
}

///////////////////////////////////////////////////////////////////////////////

Temperature::Temperature() {
}

Temperature::~Temperature() {
}

void Temperature::init(
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
			make_shared<DarkSkyWrapper>()
		);

	history = make_shared<TemperatureHistoryImpl>(
			current,
			std::make_shared<CsvTemperatureHistoryPersister>(
					make_shared<CsvReaderFactoryImpl>(temperatureHistoryProperties.fileName),
					make_shared<CsvWriterFactoryImpl>(temperatureHistoryProperties.fileName)
				),
			temperatureHistoryProperties.length
		);

	historyLogger = make_shared<TemperatureHistoryLogger>(
			history,
			make_shared<CsvWriterFactoryImpl>(temperatureHistoryLoggerProperties.fileName)
		);

	current->start(currentTemperatureProperties.updatePeriod, currentTemperatureProperties.delayOnFailed);
	forecast->start(temperatureForecastProperties.updatePeriod, temperatureForecastProperties.delayOnFailed);
	history->registerToListener();
	historyLogger->start();
}

void Temperature::uninit() {
	historyLogger->stop();
	history->unregisterFromListener();
	forecast->stop();
	current->stop();
}

const shared_ptr<TemperatureHistory> Temperature::getTemperatureHistory() const {
	return history;
}

const shared_ptr<TemperatureForecast> Temperature::getTemperatureForecast() const {
	return forecast;
}

shared_ptr<CurrentTemperatureProvider> Temperature::createCurrentTemperatureProvider() {

	try {
		return make_shared<DS18B20Wrapper>();
	} catch (const exception& e) {
		LOGGER.warning("Can not initialize DS18B20 temperature sensor", e);
	}

	return make_shared<DarkSkyWrapper>();
}
