#include "ZoneHandler.h"
#include "ZoneHandlerImpl.h"
#include "ValveConfig.h"
#include <limits>

using namespace std;


const size_t ZoneHandler::invalidZoneId = numeric_limits<size_t>::max();


const shared_ptr<ZoneHandler> ZoneHandler::getInstancePtr() {
	static shared_ptr<ZoneHandler> instance = ZoneHandlerImpl::Builder().build();
	return instance;
}

size_t ZoneHandler::getZoneCount() {
	return VALVE_COUNT - 1;
}
