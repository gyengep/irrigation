#include "TimePeriod.h"

using namespace std;
using namespace chrono;


pair<system_clock::time_point, system_clock::time_point>
getPreviousPeriod(const system_clock::time_point& currentTime, const seconds& periodLength) {
	const time_t currentRawTime = system_clock::to_time_t(currentTime);
	const time_t periodInSeconds = periodLength.count();

	const time_t from = ((currentRawTime / periodInSeconds) - 1) * periodInSeconds;
	const time_t to = (currentRawTime / periodInSeconds) * periodInSeconds - 1;

	return make_pair(
			system_clock::from_time_t(from),
			system_clock::from_time_t(to)
		);
}

pair<system_clock::time_point, system_clock::time_point>
getCurrentPeriod(const system_clock::time_point& currentTime, const seconds& periodLength) {
	const time_t currentRawTime = system_clock::to_time_t(currentTime);
	const time_t periodInSeconds = periodLength.count();

	const time_t from = (currentRawTime / periodInSeconds) * periodInSeconds;
	const time_t to = ((currentRawTime / periodInSeconds) + 1) * periodInSeconds - 1;

	return make_pair<system_clock::time_point, system_clock::time_point>(
			system_clock::from_time_t(from),
			system_clock::from_time_t(to)
		);
}
