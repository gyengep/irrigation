#include "TemperatureHistoryPersister.h"



TemperatureHistoryPersister::Sample::Sample(const std::time_t& rawTime, float temperature) :
	rawTime(rawTime),
	temperature(temperature)
{
}

bool TemperatureHistoryPersister::Sample::operator== (const TemperatureHistoryPersister::Sample& other) const {
	return (rawTime == other.rawTime && temperature == other.temperature);
}

std::ostream& operator<<(std::ostream& os, const TemperatureHistoryPersister::Sample& temperatureSample) {
	os << "TemperatureSample{";
	os << "time: " << temperatureSample.rawTime << ", ";
	os << "value: " << temperatureSample.temperature;
	os << "}";
	return os;
}
