#include "TemperatureDependentSchedulerDTO.h"
#include "EqualsPtr.h"
#include "PrintToStream.h"
#include <stdexcept>

using namespace std;


TemperatureDependentSchedulerDTO::TemperatureDependentSchedulerDTO(const TemperatureDependentSchedulerDTO& other) {

	if (other.hasRemainingCorrection()) {
		setRemainingCorrection(other.getRemainingCorrection());
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
		float remainingCorrection,
		int minAdjustment, int maxAdjustment,
		int trim)
{
	setRemainingCorrection(remainingCorrection);
	setMinAdjustment(minAdjustment);
	setMaxAdjustment(maxAdjustment);
	setTrim(trim);
}

bool TemperatureDependentSchedulerDTO::operator== (const TemperatureDependentSchedulerDTO& other) const {
	return (equalsPtr(RemainingCorrection.get(), other.RemainingCorrection.get()) &&
			equalsPtr(MinAdjustment.get(), other.MinAdjustment.get()) &&
			equalsPtr(MaxAdjustment.get(), other.MaxAdjustment.get()) &&
			equalsPtr(Trim.get(), other.Trim.get()));
}

IMPLEMENT_DTO_VALUE_COPY(TemperatureDependentSchedulerDTO, float, RemainingCorrection);
IMPLEMENT_DTO_VALUE_COPY(TemperatureDependentSchedulerDTO, int, MinAdjustment);
IMPLEMENT_DTO_VALUE_COPY(TemperatureDependentSchedulerDTO, int, MaxAdjustment);
IMPLEMENT_DTO_VALUE_COPY(TemperatureDependentSchedulerDTO, int, Trim);


ostream& operator<<(ostream& os, const TemperatureDependentSchedulerDTO& scheduler) {
	os << "TemperatureDependentSchedulerDTO{";
	PRINT_PTR(os, "remainingCorrection", scheduler.RemainingCorrection.get());
	os << ", ";
	PRINT_PTR(os, "minAdjustment", scheduler.MinAdjustment.get());
	os << ", ";
	PRINT_PTR(os, "maxAdjustment", scheduler.MaxAdjustment.get());
	os << ", ";
	PRINT_PTR(os, "trim", scheduler.Trim.get());
	os << "}";

	return os;
}
