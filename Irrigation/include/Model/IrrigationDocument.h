#pragma once
#include <memory>
#include <mutex>
#include <string>
#include "DocumentView/Document.h"
#include "DTO/DocumentDTO.h"
#include "Logic/ProgramContainer.h"
#include "Logic/WateringController.h"

class DtoReaderWriterFactory;
class FileReaderWriterFactory;


class IrrigationDocument : public Document {

	mutable std::mutex mtx;
	std::unique_ptr<ProgramContainer> programs;
	WateringController wateringController;
	std::unique_ptr<DtoReaderWriterFactory> dtoReaderWriterFactory;
	std::unique_ptr<FileReaderWriterFactory> fileReaderWriterFactory;

	IrrigationDocument(
			std::shared_ptr<ZoneHandler> zoneHandler,
			DtoReaderWriterFactory* readerWriterFactory,
			FileReaderWriterFactory* fileReaderWriterFactory);

public:
	IrrigationDocument();
	IrrigationDocument(std::shared_ptr<ZoneHandler> zoneHandler);
	IrrigationDocument(DtoReaderWriterFactory* readerWriterFactory, FileReaderWriterFactory* fileReaderWriterFactory);
	virtual ~IrrigationDocument();

	void lock() const;
	void unlock() const;

	const ProgramContainer& getPrograms() const { return *programs; }
	ProgramContainer& getPrograms() { return *programs; }

	const WateringController& getWateringController() const { return wateringController; }
	WateringController& getWateringController() { return wateringController; }

	DocumentDTO getDocumentDTO() const;
	void updateFromDTO(const DocumentDTO& documentDTO);

	void load(const std::string& fileName);
	void save(const std::string& fileName) const;
};
