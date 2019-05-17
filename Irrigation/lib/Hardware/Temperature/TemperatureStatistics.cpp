#include "TemperatureStatistics.h"
#include "TemperatureFileHandler.h"
#include "Temperature.h"
#include "Logger/Logger.h"

using namespace std;


TemperatureStatistics::TemperatureStatistics(const shared_ptr<Temperature>& temperature, const shared_ptr<TemperatureFileHandler>& temperatureFileHandler) :
	temperature(temperature),
	temperatureFileHandler(temperatureFileHandler),
	timer(*this, chrono::seconds(60) * 10)
{
}

TemperatureStatistics::~TemperatureStatistics() {
}

void TemperatureStatistics::start() {
	timer.start();
	temperatureFileHandler->start();
}

void TemperatureStatistics::stop() {
	temperatureFileHandler->stop();
	timer.stop();
}

void TemperatureStatistics::requestTemperature() {
	unique_lock<Temperature> lock(*temperature);

	const time_t currentTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
	if (temperature->isValid()) {
		temperatureFileHandler->append(currentTime, temperature->getCachedValue());
	} else {
		temperatureFileHandler->appendInvalid(currentTime);
	}
}

void TemperatureStatistics::onTimer() {
	requestTemperature();
}
