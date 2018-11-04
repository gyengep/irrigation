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
	std::unique_ptr<ProgramContainer> programs;
	std::unique_ptr<WateringController> wateringController;
	std::unique_ptr<DtoReaderWriterFactory> dtoReaderWriterFactory;
	std::unique_ptr<FileReaderWriterFactory> fileReaderWriterFactory;

public:
	IrrigationDocument(
			std::unique_ptr<ProgramContainer>&& programContainer,
			std::unique_ptr<WateringController>&& wateringController,
			std::unique_ptr<DtoReaderWriterFactory>&& dtoReaderWriterFactory,
			std::unique_ptr<FileReaderWriterFactory>&& fileReaderWriterFactory);

	virtual ~IrrigationDocument();

	void lock() const;
	void unlock() const;

	const ProgramContainer& getPrograms() const { return *programs; }
	ProgramContainer& getPrograms() { return *programs; }

	const WateringController& getWateringController() const { return *wateringController; }
	WateringController& getWateringController() { return *wateringController; }

	DocumentDTO getDocumentDTO() const;
	void updateFromDTO(const DocumentDTO& documentDTO);

	void load(const std::string& fileName);
	void save(const std::string& fileName) const;
};

class IrrigationDocument::Builder {
	std::unique_ptr<ProgramContainer> programContainer;
	std::unique_ptr<WateringController> wateringController;
	std::unique_ptr<DtoReaderWriterFactory> dtoReaderWriterFactory;
	std::unique_ptr<FileReaderWriterFactory> fileReaderWriterFactory;

public:
	Builder();
	~Builder();

	Builder& setProgramContainer(std::unique_ptr<ProgramContainer>&& programContainer);
	Builder& setWateringController(std::unique_ptr<WateringController>&& wateringController);
	Builder& setDtoReaderWriterFactory(std::unique_ptr<DtoReaderWriterFactory>&& dtoReaderWriterFactory);
	Builder& setFileReaderWriterFactory(std::unique_ptr<FileReaderWriterFactory>&& fileReaderWriterFactory);

	Builder& setWateringControllerWithFakeValves();

	std::unique_ptr<IrrigationDocument> build();
};
