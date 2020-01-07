#include "Temperature.h"
#include "DS18B20Handler.h"
#include "DarkSkyHandler.h"
#include "OWMHandler.h"
#include "CurrentTemperatureImpl.h"
#include "TemperatureHistoryImpl.h"
#include "TemperatureHistoryPersister.h"
#include "TemperatureForecastImpl.h"
#include "Logger/Logger.h"
#include "Utils/CsvReaderImpl.h"
#include "Utils/CsvWriterImpl.h"
#include "Utils/TimeConversion.h"

using namespace std;

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
		const chrono::duration<int64_t>& currentTemperatureUpdatePeriod,
		const string& temperatureHistoryFileName,
		const chrono::duration<int64_t>& temperatureHistoryLength,
		const string& temperatureHistoryPersisterFileName,
		const chrono::duration<int64_t>& temperatureHistoryPersisterPeriod,
		const chrono::duration<int64_t>& forecastUpdatePeriod
) {
	current = make_shared<CurrentTemperatureImpl>(
			createCurrentTemperatureProvider()
		);

	forecast = make_shared<TemperatureForecastImpl>(
			make_shared<DarkSkyHandler>()
		);

	history = make_shared<TemperatureHistoryImpl>(
			current,
			temperatureHistoryLength,
			temperatureHistoryFileName,
			make_shared<CsvReaderImplFactory>(),
			make_shared<CsvWriterImplFactory>()
		);

	historyPersister = make_shared<TemperatureHistoryPersister>(
			history,
			make_shared<CsvWriterImplFactory>(),
			temperatureHistoryPersisterFileName
		);

	current->updateCache();
	history->updateCache();
	forecast->updateCache();

	current->startTimer(currentTemperatureUpdatePeriod);
	history->startTimer();
	historyPersister->startTimer(temperatureHistoryPersisterPeriod);
	forecast->startTimer(forecastUpdatePeriod);
}

void Temperature::uninit() {
	forecast->stopTimer();
	historyPersister->stopTimer();
	history->stopTimer();
	current->stopTimer();
}

const shared_ptr<TemperatureHistory> Temperature::getTemperatureHistory() const {
	return history;
}

const shared_ptr<TemperatureForecast> Temperature::getTemperatureForecast() const {
	return forecast;
}

shared_ptr<CurrentTemperatureProvider> Temperature::createCurrentTemperatureProvider() {

	try {
		return make_shared<DS18B20Handler>();
	} catch (const exception& e) {
		LOGGER.warning("Can not initialize DS18B20 temperature sensor", e);
	}

	return make_shared<DarkSkyHandler>();
}
