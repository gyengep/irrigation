#include "TemperatureDependentSchedulerDTO.h"
#include "EqualsPtr.h"
#include "PrintToStream.h"
#include <stdexcept>

using namespace std;


TemperatureDependentSchedulerDTO::TemperatureDependentSchedulerDTO(const TemperatureDependentSchedulerDTO& other) {

	if (other.hasRemainingA()) {
		setRemainingA(other.getRemainingA());
	}

	if (other.hasForecastA()) {
		setForecastA(other.getForecastA());
	}

	if (other.hasForecastB()) {
		setForecastB(other.getForecastB());
	}

	if (other.hasHistoryA()) {
		setHistoryA(other.getHistoryA());
	}

	if (other.hasHistoryB()) {
		setHistoryB(other.getHistoryB());
	}

	if (other.hasMinAdjustment()) {
		setMinAdjustment(other.getMinAdjustment());
	}

	if (other.hasMaxAdjustment()) {
		setMaxAdjustment(other.getMaxAdjustment());
	}

	if (other.hasTrim()) {
		setTrim(other.getTrim());
	}
}

TemperatureDependentSchedulerDTO::TemperatureDependentSchedulerDTO(
		float remainingA,
		float forecastA, float forecastB,
		float historyA, float historyB,
		int minAdjustment, int maxAdjustment,
		int trim)
{
	setRemainingA(remainingA);
	setForecastA(forecastA);
	setForecastB(forecastB);
	setHistoryA(historyA);
	setHistoryB(historyB);
	setMinAdjustment(minAdjustment);
	setMaxAdjustment(maxAdjustment);
	setTrim(trim);
}

bool TemperatureDependentSchedulerDTO::operator== (const TemperatureDependentSchedulerDTO& other) const {
	return (equalsPtr(RemainingA.get(), other.RemainingA.get()) &&
			equalsPtr(ForecastA.get(), other.ForecastA.get()) &&
			equalsPtr(ForecastB.get(), other.ForecastB.get()) &&
			equalsPtr(HistoryA.get(), other.HistoryA.get()) &&
			equalsPtr(HistoryB.get(), other.HistoryB.get()) &&
			equalsPtr(MinAdjustment.get(), other.MinAdjustment.get()) &&
			equalsPtr(MaxAdjustment.get(), other.MaxAdjustment.get()) &&
			equalsPtr(Trim.get(), other.Trim.get()));
}

IMPLEMENT_DTO_VALUE_COPY(TemperatureDependentSchedulerDTO, float, RemainingA);
IMPLEMENT_DTO_VALUE_COPY(TemperatureDependentSchedulerDTO, float, ForecastA);
IMPLEMENT_DTO_VALUE_COPY(TemperatureDependentSchedulerDTO, float, ForecastB);
IMPLEMENT_DTO_VALUE_COPY(TemperatureDependentSchedulerDTO, float, HistoryA);
IMPLEMENT_DTO_VALUE_COPY(TemperatureDependentSchedulerDTO, float, HistoryB);
IMPLEMENT_DTO_VALUE_COPY(TemperatureDependentSchedulerDTO, int, MinAdjustment);
IMPLEMENT_DTO_VALUE_COPY(TemperatureDependentSchedulerDTO, int, MaxAdjustment);
IMPLEMENT_DTO_VALUE_COPY(TemperatureDependentSchedulerDTO, int, Trim);


ostream& operator<<(ostream& os, const TemperatureDependentSchedulerDTO& scheduler) {
	os << "TemperatureDependentSchedulerDTO{";
	PRINT_PTR(os, "remainingA", scheduler.RemainingA.get());
	os << ", ";
	PRINT_PTR(os, "forecastA", scheduler.ForecastA.get());
	os << ", ";
	PRINT_PTR(os, "forecastB", scheduler.ForecastB.get());
	os << ", ";
	PRINT_PTR(os, "historyA", scheduler.HistoryA.get());
	os << ", ";
	PRINT_PTR(os, "historyB", scheduler.HistoryB.get());
	os << ", ";
	PRINT_PTR(os, "minAdjustment", scheduler.MinAdjustment.get());
	os << ", ";
	PRINT_PTR(os, "maxAdjustment", scheduler.MaxAdjustment.get());
	os << ", ";
	PRINT_PTR(os, "trim", scheduler.Trim.get());
	os << "}";

	return os;
}
