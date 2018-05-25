#pragma once
#include <ctime>
#include "DocumentView/Document.h"
#include "Logic/ProgramContainer.h"
#include "Logic/WateringController.h"


class IrrigationDocument : public Document {

	ProgramContainer programs;
	WateringController wateringController;

	bool programContainerCallback(const IdType& id, LockedProgramPtr lockedProgram, const time_t& rawTime);

public:

	IrrigationDocument();
	virtual ~IrrigationDocument();

	const ProgramContainer& getPrograms() const { return programs; }
	ProgramContainer& getPrograms() { return programs; }

	const WateringController& getWateringController() const { return wateringController; }
	WateringController& getWateringController() { return wateringController; }

	void on1SecTimer(const std::time_t& rawTime);

	virtual void load();
};
