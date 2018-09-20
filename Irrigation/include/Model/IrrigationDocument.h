#pragma once
#include <memory>
#include <mutex>
#include "DocumentView/Document.h"
#include "DTO/DocumentDTO.h"
#include "Logic/ProgramContainer.h"
#include "Logic/WateringController.h"


class IrrigationDocument : public Document {

	mutable std::mutex mutex;
	std::unique_ptr<ProgramContainer> programs;
	WateringController wateringController;

public:
	IrrigationDocument();
	IrrigationDocument(std::shared_ptr<ZoneHandler> zoneHandler);
	virtual ~IrrigationDocument();

	void lock() const;
	void unlock() const;

	const ProgramContainer& getPrograms() const { return *programs; }
	ProgramContainer& getPrograms() { return *programs; }

	const WateringController& getWateringController() const { return wateringController; }
	WateringController& getWateringController() { return wateringController; }

	DocumentDTO getDocumentDTO() const;
	void updateFromDTO(const DocumentDTO& documentDTO);
};
