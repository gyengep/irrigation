#include "ZoneHandler.h"
#include "GpioValve.h"
#include "FakeValve.h"
#include "ValveConfig.h"
#include "Valve.h"
#include <vector>
#include "Exceptions/Exceptions.h"

using namespace std;


const shared_ptr<ZoneHandler> ZoneHandler::getInstancePtr() {
	static shared_ptr<ZoneHandler> instance = Builder().build();
	return instance;
}

size_t ZoneHandler::getZoneCount() {
	return VALVE_COUNT - 1;
}

///////////////////////////////////////////////////////////////////////////////

ZoneHandler::ZoneHandler(unique_ptr<ValveFactory>&& valveFactory) :
	zoneValves(getZoneCount()),
	activeZoneId(invalidZoneId)
{
	for (size_t i = 0; i < zoneValves.size(); i++) {
		zoneValves[i] = valveFactory->createValve(i);
	}

	masterValve = valveFactory->createValve(getZoneCount());
}

ZoneHandler::~ZoneHandler() {
	deactivate();
}

size_t ZoneHandler::getActiveId() const {
	return activeZoneId;
}

void ZoneHandler::activate(size_t zoneId) {
	if (zoneValves.size() <= zoneId) {
		throw IndexOutOfBoundsException(
				"Zone index shall be less than " + to_string(zoneValves.size()) +
				", while actual value is " + to_string(zoneId));
	}

	deactivate();

	activeZoneId = zoneId;
	zoneValves[activeZoneId]->activate();
	masterValve->activate();
}

void ZoneHandler::deactivate() {
	if (invalidZoneId != activeZoneId) {
		zoneValves[activeZoneId]->deactivate();
		masterValve->deactivate();
		activeZoneId = invalidZoneId;
	}
}

///////////////////////////////////////////////////////////////////////////////

ZoneHandler::Builder& ZoneHandler::Builder::setValveFactory(unique_ptr<ValveFactory>&& valveFactory) {
	this->valveFactory = move(valveFactory);
	return *this;
}

ZoneHandler::Builder& ZoneHandler::Builder::setFakeValveFactory() {
	valveFactory = unique_ptr<ValveFactory>(new FakeValveFactory());
	return *this;
}

shared_ptr<ZoneHandler> ZoneHandler::Builder::build() {
	if (nullptr == valveFactory) {
		valveFactory = unique_ptr<ValveFactory>(new GpioValveFactory());
	}

	return shared_ptr<ZoneHandler>(new ZoneHandler(
			move(valveFactory)));
}
