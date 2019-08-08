#include "FixedAmountScheduler.h"
#include "Logger/Logger.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

FixedAmountScheduler::FixedAmountScheduler(const shared_ptr<TemperatureForecast>& temperatureForecast, const shared_ptr<TemperatureHistory>& temperatureHistory) :
	TemperatureDependentScheduler(temperatureForecast, temperatureHistory)
{
}

FixedAmountScheduler::~FixedAmountScheduler() {
}

int FixedAmountScheduler::onCalculateAdjustment(const time_t rawTime) {
	const int requiredPercentForNextDay = getRequiredPercentForNextDay(rawTime);
	LOGGER.trace("%-30s%d%%", "requiredPercentForNextDay", requiredPercentForNextDay);

	if (requiredPercentForNextDay > getRemainingPercent()) {
		return max(requiredPercentForNextDay, 100);
	} else {
		return 0;
	}
}
