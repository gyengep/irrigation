#pragma once
#include <atomic>
#include <memory>
#include <mutex>
#include "json.hpp"
#include "DTO/DocumentDTO.h"
#include "Email/EmailHandler.h"
#include "Logic/Program.h"
#include "Logic/ProgramContainer.h"
#include "Logic/WateringController.h"
#include "IrrigationDocument.h"


class IrrigationDocumentImpl : public IrrigationDocument {
public:
	class Builder;

private:
	const std::shared_ptr<ProgramContainer> programContainer;
	const std::shared_ptr<ProgramFactory> programFactory;
	const std::shared_ptr<WateringController> wateringController;
	const std::shared_ptr<EmailHandler> emailHandler;

	mutable std::mutex mtx;
	std::atomic_bool modified;

	void start(const ProgramPtr& program, unsigned adjustment);
	void start(const DurationList& durations, unsigned adjustment);

public:
	IrrigationDocumentImpl(
		const std::shared_ptr<ProgramContainer>& programContainer,
		const std::shared_ptr<ProgramFactory>& programFactory,
		const std::shared_ptr<WateringController>& wateringController,
		const std::shared_ptr<EmailHandler>& emailHandler
	);

	virtual ~IrrigationDocumentImpl();

	virtual void lock() const override;
	virtual void unlock() const override;

	virtual const ProgramContainer& getProgramContainer() const override;
	virtual ProgramContainer& getProgramContainer() override;

	virtual std::pair<IdType, ProgramPtr> createProgram(const ProgramDTO& programDto) override;

	virtual bool isModified() const override;
	virtual void setModified(bool modified = true) override;

	virtual void startIfScheduled(const LocalDateTime& localDateTime) override;
	virtual void startProgram(const IdType& programId, unsigned adjustment) override;
	virtual void startCustom(const DurationList& durations, unsigned adjustment) override;
	virtual void stop() override;

	virtual DocumentDTO toDocumentDto() const override;
	virtual void updateFromDocumentDto(const std::shared_ptr<ProgramFactory>& programFactory, const DocumentDTO& documentDTO) override;

	virtual void saveState() const override;
	virtual void loadState() override;

	nlohmann::json saveTo() const;
	void loadFrom(const nlohmann::json& values);
};

class IrrigationDocumentImpl::Builder {
	std::shared_ptr<ProgramContainer> programContainer;
	std::shared_ptr<ProgramFactory> programFactory;
	std::shared_ptr<WateringController> wateringController;
	std::shared_ptr<EmailHandler> emailHandler;

public:

	Builder& setProgramContainer(const std::shared_ptr<ProgramContainer>& programContainer);
	Builder& setProgramFactory(const std::shared_ptr<ProgramFactory>& programFactory);
	Builder& setWateringController(const std::shared_ptr<WateringController>& wateringController);
	Builder& setEmailHandler(const std::shared_ptr<EmailHandler>& emailHandler);

	std::shared_ptr<IrrigationDocumentImpl> build();
};
