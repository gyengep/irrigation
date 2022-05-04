#include "ZoneHandlerImpl.h"
#include "ValveConfig.h"
#include <limits>

using namespace std;


const size_t ZoneHandler::invalidZoneId = numeric_limits<size_t>::max();


size_t ZoneHandler::getZoneCount() {
	return VALVE_COUNT - 1;
}
