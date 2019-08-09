#include "TemperatureToPercent.h"

using namespace std;

TemperatureToPercent& TemperatureToPercent::getInstance() {
	static TemperatureToPercent instance;
	return instance;
}

TemperatureToPercent::TemperatureToPercent() {
	temperatureAndPercents = vector<pair<float, int>>{
		{ 15.0f, 25 },
		{ 25.0f, 50 },
		{ 35.0f, 100 }
	};
}

TemperatureToPercent::~TemperatureToPercent() {
}

void TemperatureToPercent::setTemperatureAndPercents(const vector<pair<float, int>>& temperatureAndPercents) {
	this->temperatureAndPercents = temperatureAndPercents;
}

int TemperatureToPercent::getRequiredPercentFromTemperature(float temperature) const {
	if (temperature < temperatureAndPercents.front().first) {
		return 0;
	}

	for (auto it = next(temperatureAndPercents.begin()); it != temperatureAndPercents.end(); ++it) {
		if (prev(it)->first <= temperature && temperature < it->first) {
			const double t0 = prev(it)->first;
			const double t1 = it->first;
			const double p0 = prev(it)->second;
			const double p1 = it->second;
			const double ratio = (temperature - t0) / (t1 - t0);
			return ((p1 - p0) * ratio ) + p0;
		}
	}

//	return temperatureAndPercents.back().second;
	const auto prev1 = prev(temperatureAndPercents.end());
	const auto prev2 = prev(prev1);

	const double t0 = prev2->first;
	const double t1 = prev1->first;
	const double p0 = prev2->second;
	const double p1 = prev1->second;
	const double ratio = (temperature - t0) / (t1 - t0);
	return ((p1 - p0) * ratio ) + p0;
}

