#include "TemperatureStatistics.h"
#include "TemperatureException.h"
#include "TemperaturePersister.h"
#include "TemperatureSensor.h"
#include "Logger/Logger.h"

using namespace std;


TemperatureStatistics::TemperatureStatistics(const shared_ptr<TemperatureSensor>& temperatureSensor, const shared_ptr<TemperaturePersister>& temperaturePersister) :
	temperatureSensor(temperatureSensor),
	temperaturePersister(temperaturePersister),
	timer(*this, chrono::seconds(60) * 10)
{
	temperaturePersister->startTimer();
}

TemperatureStatistics::~TemperatureStatistics() {
	temperaturePersister->stopTimer();
}

void TemperatureStatistics::requestTemperature() {
	const time_t currentTime = chrono::system_clock::to_time_t(chrono::system_clock::now());

	try {
		temperaturePersister->append(currentTime, temperatureSensor->getCachedValue());
	} catch (const TemperatureException& e) {
		temperaturePersister->appendInvalid(currentTime);
	}
}

///////////////////////////////////////////////////////////////////////////////

void TemperatureStatistics::startTimer() {
	timer.start();
}

void TemperatureStatistics::stopTimer() {
	timer.stop();
}

void TemperatureStatistics::onTimer() {
	requestTemperature();
}
