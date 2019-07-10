#include "TimePeriod.h"

using namespace std;
using namespace chrono;


pair<time_t, time_t> getPreviousPeriod(const time_t& rawTime, const seconds& periodLength) {
	const time_t periodInSeconds = periodLength.count();

	const time_t from = ((rawTime / periodInSeconds) - 1) * periodInSeconds;
	const time_t to = (rawTime / periodInSeconds) * periodInSeconds - 1;

	return make_pair(from, to);
}

pair<time_t, time_t> getCurrentPeriod(const time_t& rawTime, const seconds& periodLength) {
	const time_t periodInSeconds = periodLength.count();

	const time_t from = (rawTime / periodInSeconds) * periodInSeconds;
	const time_t to = ((rawTime / periodInSeconds) + 1) * periodInSeconds - 1;

	return make_pair(from, to);
}
