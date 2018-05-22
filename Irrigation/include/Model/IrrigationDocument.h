#pragma once
#include "DocumentView/Document.h"
#include "Logic/ProgramContainer.h"
#include "Logic/ZoneHandler.h"


class IrrigationDocument : public Document {

	ProgramContainer programs;
	ZoneHandler zones;

public:

	IrrigationDocument();
	virtual ~IrrigationDocument();

	// Program
	const ProgramContainer& getPrograms() const { return programs; }
	ProgramContainer& getPrograms() { return programs; }

	// Zones
	const ZoneHandler& getZones() const { return zones; }
	ZoneHandler& getZones() { return zones; }

	virtual void load();
};
