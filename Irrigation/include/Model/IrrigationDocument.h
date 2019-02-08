#pragma once
#include <memory>
#include <mutex>
#include <string>
#include "DocumentView/Document.h"
#include "DTO/DocumentDTO.h"

class ProgramContainer;
class DtoReaderWriterFactory;
class FileReaderWriterFactory;
class WateringController;


class IrrigationDocument : public Document {
public:
	class Builder;

private:
	mutable std::mutex mtx;
	std::shared_ptr<ProgramContainer> programs;
	std::shared_ptr<WateringController> wateringController;
	std::shared_ptr<DtoReaderWriterFactory> dtoReaderWriterFactory;
	std::shared_ptr<FileReaderWriterFactory> fileReaderWriterFactory;

public:
	IrrigationDocument(
			std::shared_ptr<ProgramContainer> programContainer,
			std::shared_ptr<WateringController> wateringController,
			std::shared_ptr<DtoReaderWriterFactory> dtoReaderWriterFactory,
			std::shared_ptr<FileReaderWriterFactory> fileReaderWriterFactory
			);

	IrrigationDocument(const IrrigationDocument&);

	virtual ~IrrigationDocument();

	void lock() const;
	void unlock() const;

	const ProgramContainer& getPrograms() const { return *programs; }
	ProgramContainer& getPrograms() { return *programs; }

	const WateringController& getWateringController() const { return *wateringController; }
	WateringController& getWateringController() { return *wateringController; }

	DocumentDTO toDocumentDto() const;
	void updateFromDocumentDto(const DocumentDTO& documentDTO);

	void load(const std::string& fileName);
	void save(const std::string& fileName) const;
};

class IrrigationDocument::Builder {
	std::shared_ptr<ProgramContainer> programContainer;
	std::shared_ptr<WateringController> wateringController;
	std::shared_ptr<DtoReaderWriterFactory> dtoReaderWriterFactory;
	std::shared_ptr<FileReaderWriterFactory> fileReaderWriterFactory;

public:
	Builder();
	~Builder();

	Builder& setProgramContainer(std::shared_ptr<ProgramContainer> programContainer);
	Builder& setWateringController(std::shared_ptr<WateringController> wateringController);
	Builder& setDtoReaderWriterFactory(std::shared_ptr<DtoReaderWriterFactory> dtoReaderWriterFactory);
	Builder& setFileReaderWriterFactory(std::shared_ptr<FileReaderWriterFactory> fileReaderWriterFactory);

	std::shared_ptr<IrrigationDocument> build();
};
