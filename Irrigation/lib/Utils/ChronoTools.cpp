#include "ChronoTools.h"
#include <cstdlib>

using namespace std;
using namespace std::chrono;


TimeConverter::TimeConverter(milliseconds ms) {
	days = duration_cast<Days>(ms);
	hours = duration_cast<chrono::hours>(ms - days);
	minutes = duration_cast<chrono::minutes>(ms - days - hours);
	seconds = duration_cast<chrono::seconds>(ms - days - hours - minutes);
	millis = duration_cast<chrono::milliseconds>(ms - days - hours - minutes - seconds);
}

seconds abs(const seconds& t) {
	return seconds(abs(t.count()));
}

milliseconds abs(const milliseconds& t) {
	return milliseconds(abs(t.count()));
}
