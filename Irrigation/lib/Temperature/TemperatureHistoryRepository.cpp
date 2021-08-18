#include "TemperatureHistoryRepository.h"



TemperatureHistoryRepository::Sample::Sample(const DateTime& dateTime, float temperature) :
	dateTime(dateTime),
	temperature(temperature)
{
}

bool TemperatureHistoryRepository::Sample::operator== (const TemperatureHistoryRepository::Sample& other) const {
	return (dateTime == other.dateTime && temperature == other.temperature);
}

std::ostream& operator<<(std::ostream& os, const TemperatureHistoryRepository::Sample& temperatureSample) {
	os << "TemperatureSample{";
	os << "time: " << LocalDateTime(temperatureSample.dateTime).toString() << ", ";
	os << "value: " << temperatureSample.temperature;
	os << "}";
	return os;
}
