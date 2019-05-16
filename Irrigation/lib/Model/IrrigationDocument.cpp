#include "IrrigationDocument.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"
#include "Logic/RunTime.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTime.h"
#include "Logic/StartTimeContainer.h"
#include "Logic/ProgramContainer.h"
#include "Logic/WateringController.h"

using namespace std;


IrrigationDocument::IrrigationDocument(
		shared_ptr<ProgramContainer> programContainer,
		shared_ptr<WateringController> wateringController) :
	programs(programContainer),
	wateringController(wateringController),
	modified(false)
{
}

IrrigationDocument::~IrrigationDocument() {
}

void IrrigationDocument::lock() const {
	mtx.lock();
}

void IrrigationDocument::unlock() const {
	mtx.unlock();
}

DocumentDTO IrrigationDocument::toDocumentDto() const {
	return DocumentDTO(programs->toProgramDtoList());
}

void IrrigationDocument::updateFromDocumentDto(const DocumentDTO& documentDTO) {
	if (documentDTO.hasPrograms()) {
		programs->updateFromProgramDtoList(documentDTO.getPrograms());
	}

	if (LOGGER.isLoggable(LogLevel::DEBUG)) {
		for (const auto& programWithId : getPrograms()) {
			LOGGER.debug("Program[%s] is added: %s", to_string(programWithId.first).c_str(), to_string(*programWithId.second).c_str());
		}

 		LOGGER.debug("Configuration is successfully loaded");
	}
}

///////////////////////////////////////////////////////////////////////////////

IrrigationDocument::Builder::Builder() {
}

IrrigationDocument::Builder::~Builder() {
}

IrrigationDocument::Builder& IrrigationDocument::Builder::setProgramContainer(shared_ptr<ProgramContainer> programContainer) {
	this->programContainer = programContainer;
	return *this;
}

IrrigationDocument::Builder& IrrigationDocument::Builder::setWateringController(shared_ptr<WateringController> wateringController) {
	this->wateringController = wateringController;
	return *this;
}

shared_ptr<IrrigationDocument> IrrigationDocument::Builder::build() {

	if (nullptr == programContainer) {
		programContainer.reset(new ProgramContainer());
	}

	if (nullptr == wateringController) {
		wateringController.reset(new WateringController());
	}

	return shared_ptr<IrrigationDocument>(new IrrigationDocument(
			programContainer,
			wateringController));
}
