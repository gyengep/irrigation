#include "FixedPeriodScheduler.h"
#include "Logger/Logger.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

FixedPeriodScheduler::FixedPeriodScheduler(const shared_ptr<TemperatureForecast>& temperatureForecast, const shared_ptr<TemperatureHistory>& temperatureHistory) :
	TemperatureDependentScheduler(temperatureForecast, temperatureHistory)
{
	setUseRemainingWithPercent(50);
}

FixedPeriodScheduler::~FixedPeriodScheduler() {
}

int FixedPeriodScheduler::onCalculateAdjustment(const time_t rawTime) {
	const int requiredPercentForNextDay = getRequiredPercentForNextDay(rawTime);
	LOGGER.trace("%-30s%d%%", "requiredPercentForNextDay", requiredPercentForNextDay);

	return (requiredPercentForNextDay - getRemainingPercent());
}
