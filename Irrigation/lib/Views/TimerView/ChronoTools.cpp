#include "ChronoTools.h"
#include <cstdlib>


using namespace std;

TimeConverter::TimeConverter(std::chrono::milliseconds ms) {
	days = chrono::duration_cast<Days>(ms);
	hours = chrono::duration_cast<chrono::hours>(ms - days);
	minutes = chrono::duration_cast<chrono::minutes>(ms - days - hours);
	seconds = chrono::duration_cast<chrono::seconds>(ms - days - hours - minutes);
}



chrono::milliseconds getDiffBetweenSystemClockAndSteadyClock() {
	chrono::milliseconds steady = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch());
	chrono::milliseconds system = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());

	chrono::milliseconds::rep msSteady = chrono::milliseconds(steady).count();
	chrono::milliseconds::rep msSystem = chrono::milliseconds(system).count();

	return chrono::milliseconds(msSystem - msSteady);
}

chrono::milliseconds abs(const chrono::milliseconds& ms) {
	return chrono::milliseconds(abs(ms.count()));
}

