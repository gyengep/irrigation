#pragma once
#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include "json.hpp"
#include "DocumentView/Document.h"
#include "DTO/DocumentDTO.h"
#include "Logic/Program.h"
#include "Logic/ProgramContainer.h"

class WateringController;


class IrrigationDocument : public Document {
public:
	class Builder;

private:
	mutable std::mutex mtx;
	std::shared_ptr<ProgramContainer> programs;
	std::shared_ptr<ProgramFactory> programFactory;
	std::shared_ptr<WateringController> wateringController;
	std::atomic_bool modified;

public:
	IrrigationDocument(
		const std::shared_ptr<ProgramContainer>& programContainer,
		const std::shared_ptr<ProgramFactory>& programFactory,
		const std::shared_ptr<WateringController>& wateringController
	);

	virtual ~IrrigationDocument();

	void lock() const;
	void unlock() const;

	const ProgramContainer& getPrograms() const { return *programs; }
	ProgramContainer& getPrograms() { return *programs; }

	std::pair<IdType, ProgramPtr> createProgram(const ProgramDTO& programDto);

	const WateringController& getWateringController() const { return *wateringController; }
	WateringController& getWateringController() { return *wateringController; }

	bool isModified() const { return modified; }
	void setModified(bool modified = true) { this->modified = modified; }

	DocumentDTO toDocumentDto() const;
	void updateFromDocumentDto(const std::shared_ptr<ProgramFactory>& programFactory, const DocumentDTO& documentDTO);

	nlohmann::json saveTo() const;
	void loadFrom(const nlohmann::json& values);

	void saveState() const;
	void loadState();
};

class IrrigationDocument::Builder {
	std::shared_ptr<ProgramContainer> programContainer;
	std::shared_ptr<ProgramFactory> programFactory;
	std::shared_ptr<WateringController> wateringController;

public:
	Builder();
	~Builder();

	Builder& setProgramContainer(const std::shared_ptr<ProgramContainer>& programContainer);
	Builder& setProgramFactory(const std::shared_ptr<ProgramFactory>& programFactory);
	Builder& setWateringController(const std::shared_ptr<WateringController>& wateringController);

	std::shared_ptr<IrrigationDocument> build();
};
