#include "TemperatureHistoryPersister.h"



TemperatureHistoryPersister::Sample::Sample(const DateTime& dateTime, float temperature) :
	dateTime(dateTime),
	temperature(temperature)
{
}

bool TemperatureHistoryPersister::Sample::operator== (const TemperatureHistoryPersister::Sample& other) const {
	return (dateTime == other.dateTime && temperature == other.temperature);
}

std::ostream& operator<<(std::ostream& os, const TemperatureHistoryPersister::Sample& temperatureSample) {
	os << "TemperatureSample{";
	os << "time: " << LocalDateTime(temperatureSample.dateTime).toString() << ", ";
	os << "value: " << temperatureSample.temperature;
	os << "}";
	return os;
}
