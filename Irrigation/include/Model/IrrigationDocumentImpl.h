#pragma once
#include <atomic>
#include <memory>
#include <mutex>
#include "DTO/DocumentDTO.h"
#include "Email/EmailHandler.h"
#include "Logic/ProgramContainer.h"
#include "Logic/WateringController.h"
#include "IrrigationDocument.h"



class IrrigationDocumentImpl : public IrrigationDocument {
	const std::shared_ptr<Loader> documentLoader;
	const std::shared_ptr<Saver> documentSaver;
	const std::shared_ptr<ProgramContainer> programContainer;
	const std::shared_ptr<WateringController> wateringController;
	const std::shared_ptr<EmailHandler> emailHandler;

	mutable std::mutex mtx;
	mutable std::atomic_bool modified;

	void start(const DurationList& durations, unsigned adjustment);

public:
	IrrigationDocumentImpl(
		const std::shared_ptr<Loader>& documentLoader,
		const std::shared_ptr<Saver>& documentSaver,
		const std::shared_ptr<ProgramContainer>& programContainer,
		const std::shared_ptr<WateringController>& wateringController,
		const std::shared_ptr<EmailHandler>& emailHandler
	);

	virtual ~IrrigationDocumentImpl();

	virtual void lock() const override;
	virtual void unlock() const override;

	virtual const ProgramContainer& getProgramContainer() const override;
	virtual ProgramContainer& getProgramContainer() override;

	virtual bool isModified() const override;
	virtual void setModified(bool modified = true) override;

	virtual void startIfScheduled(const LocalDateTime& localDateTime) override;
	virtual void startProgram(const IdType& programId, unsigned adjustment) override;
	virtual void startCustom(const DurationList& durations, unsigned adjustment) override;
	virtual void stop() override;

	virtual DocumentDTO toDocumentDto() const override;
	virtual void updateFromDocumentDto(const DocumentDTO& documentDTO) override;

	virtual void save() const override;
	virtual void load() override;
};
