#pragma once
#include <ctime>
#include <mutex>
#include "DocumentView/Document.h"
#include "DTO/DocumentDTO.h"
#include "Logic/ProgramContainer.h"
#include "Logic/WateringController.h"


class IrrigationDocument : public Document {

	mutable std::mutex mutex;
	ProgramContainer programs;
	WateringController wateringController;

public:
	IrrigationDocument();
	virtual ~IrrigationDocument();

	void lock() const;
	void unlock() const;

	const ProgramContainer& getPrograms() const { return programs; }
	ProgramContainer& getPrograms() { return programs; }

	const WateringController& getWateringController() const { return wateringController; }
	WateringController& getWateringController() { return wateringController; }

	void on1SecTimer(const std::time_t& rawTime);

	DocumentDTO getDocumentDTO() const;
};
