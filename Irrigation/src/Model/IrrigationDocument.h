#pragma once

#include <array>
#include <mutex>

#include "DocumentView/Document.h"
#include "Logic/Containers.h"
#include "Logic/Program.h"
#include "Logic/Zones.h"


class IrrigationDocument : public Document {

	std::unique_ptr<ProgramContainer> programs;
	Zones zones;
	
public:

	IrrigationDocument();
	virtual ~IrrigationDocument();

	// Program
	const ProgramContainer& getPrograms() const { return *programs; }
	ProgramContainer& getPrograms() { return *programs; }

	// Zones
	const Zones& getZones() const { return zones; }
	Zones& getZones() { return zones; }

	virtual void load();
};
