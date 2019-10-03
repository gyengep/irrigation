#include "HotWeatherSchedulerDTO.h"
#include "EqualsPtr.h"
#include "PrintToStream.h"
#include <stdexcept>

using namespace std;


HotWeatherSchedulerDTO::HotWeatherSchedulerDTO(const HotWeatherSchedulerDTO& other) {

	if (other.hasMinTemperature()) {
		setMinTemperature(other.getMinTemperature());
	}

	if (other.hasPeriodInSeconds()) {
		setPeriodInSeconds(other.getPeriodInSeconds());
	}
}

HotWeatherSchedulerDTO::HotWeatherSchedulerDTO(unsigned periodInSeconds, float minTemperature)
{
	setMinTemperature(minTemperature);
	setPeriodInSeconds(periodInSeconds);
}

bool HotWeatherSchedulerDTO::operator== (const HotWeatherSchedulerDTO& other) const {
	return (equalsPtr(MinTemperature.get(), other.MinTemperature.get()) &&
			equalsPtr(PeriodInSeconds.get(), other.PeriodInSeconds.get()));
}

IMPLEMENT_DTO_VALUE_COPY(HotWeatherSchedulerDTO, float, MinTemperature);
IMPLEMENT_DTO_VALUE_COPY(HotWeatherSchedulerDTO, unsigned, PeriodInSeconds);


ostream& operator<<(ostream& os, const HotWeatherSchedulerDTO& scheduler) {
	os << "HotWeatherSchedulerDTO{";
	PRINT_PTR(os, "minTemerature", scheduler.MinTemperature.get());
	os << ", ";
	PRINT_PTR(os, "periodInSeconds", scheduler.PeriodInSeconds.get());
	os << "}";

	return os;
}
