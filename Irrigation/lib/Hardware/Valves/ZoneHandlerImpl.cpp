#include "ZoneHandlerImpl.h"
#include "GpioValve.h"
#include "ValveConfig.h"
#include "Exceptions/Exceptions.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

ZoneHandlerImpl::ZoneHandlerImpl(const shared_ptr<ValveFactory>& valveFactory) :
	zoneValves(getZoneCount()),
	activeZoneId(invalidZoneId)
{
	if (nullptr == valveFactory) {
		throw invalid_argument("ZoneHandlerImpl::ZoneHandlerImpl() valveFactory pointer cannot be NULL");
	}

	for (size_t i = 0; i < zoneValves.size(); i++) {
		zoneValves[i] = valveFactory->createValve(i);
	}

	masterValve = valveFactory->createValve(getZoneCount());
}

ZoneHandlerImpl::~ZoneHandlerImpl() {
	deactivate();
}

size_t ZoneHandlerImpl::getActiveId() const {
	return activeZoneId;
}

void ZoneHandlerImpl::activate(size_t zoneId) {
	if (zoneValves.size() <= zoneId) {
		throw IndexOutOfBoundsException(
				"Zone index shall be less than " + to_string(zoneValves.size()) +
				", while actual value is " + to_string(zoneId));
	}

	if (invalidZoneId != activeZoneId) {
		zoneValves[activeZoneId]->deactivate();
	} else {
		masterValve->activate();
	}

	activeZoneId = zoneId;
	zoneValves[activeZoneId]->activate();
}

void ZoneHandlerImpl::deactivate() {
	if (invalidZoneId != activeZoneId) {
		zoneValves[activeZoneId]->deactivate();
		masterValve->deactivate();
		activeZoneId = invalidZoneId;
	}
}

///////////////////////////////////////////////////////////////////////////////

ZoneHandlerImpl::Builder& ZoneHandlerImpl::Builder::setValveFactory(const shared_ptr<ValveFactory>& valveFactory) {
	this->valveFactory = valveFactory;
	return *this;
}

shared_ptr<ZoneHandlerImpl> ZoneHandlerImpl::Builder::build() {
	if (nullptr == valveFactory) {
		valveFactory = make_shared<GpioValveFactory>();
	}

	return shared_ptr<ZoneHandlerImpl>(new ZoneHandlerImpl(valveFactory));
}
