#pragma once
#include <memory>
#include "DocumentView/Document.h"
#include "DTO/DocumentDTO.h"
#include "Logic/Program.h"
#include "Logic/ProgramContainer.h"


class IrrigationDocument : public Document {

public:
	virtual ~IrrigationDocument() = default;

	virtual void lock() const = 0;
	virtual void unlock() const = 0;

	virtual const ProgramContainer& getProgramContainer() const = 0;
	virtual ProgramContainer& getProgramContainer() = 0;

	virtual bool isModified() const = 0;
	virtual void setModified(bool modified = true) = 0;

	virtual void startIfScheduled(const LocalDateTime& localDateTime) = 0;
	virtual void startProgram(const IdType& programId, unsigned adjustment) = 0;
	virtual void startCustom(const DurationList& durations, unsigned adjustment) = 0;
	virtual void stop() = 0;

	virtual DocumentDTO toDocumentDto() const = 0;
	virtual void updateFromDocumentDto(const DocumentDTO& documentDTO) = 0;

	virtual void saveState() const = 0;
	virtual void loadState() = 0;
};
