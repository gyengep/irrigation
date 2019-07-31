#include "IrrigationDocument.h"
#include "Configuration.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"
#include "Logic/RunTime.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTime.h"
#include "Logic/StartTimeContainer.h"
#include "Logic/ProgramContainer.h"
#include "Logic/WateringController.h"
#include <algorithm>
#include <fstream>
#include <iomanip>

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

nlohmann::json IrrigationDocument::saveTo() const {
	nlohmann::json result;

	for (const auto& programWithId : getPrograms()) {
		const string key = "program_" + to_string(programWithId.first);
		result[key] = programWithId.second->saveTo();
	}

	return result;
}

void IrrigationDocument::loadFrom(const nlohmann::json& values) {
	for (const auto& programWithId : getPrograms()) {
		const string key = "program_" + to_string(programWithId.first);

		auto it = values.find(key);
		if (values.end() != it) {
			programWithId.second->loadFrom(it.value());
		}
	}
}

void IrrigationDocument::saveState() const {
	LOGGER.trace("IrrigationDocument::saveState() >>>>>");
	ofstream o(Configuration::getInstance().getPersistedDatFileName());
	o << setw(4) << saveTo() << endl;
	LOGGER.debug("Document state is saved");
	LOGGER.trace("<<<<< IrrigationDocument::saveState()");
}

void IrrigationDocument::loadState() {
	LOGGER.trace("IrrigationDocument::loadState() >>>>>");
	std::ifstream i(Configuration::getInstance().getPersistedDatFileName());
	if (i.is_open()) {
		nlohmann::json j;
		i >> j;
		loadFrom(j);
		LOGGER.debug("Document state is loaded");
	} else {
		LOGGER.debug("Document state is NOT loaded");
	}
	LOGGER.trace("<<<<< IrrigationDocument::loadState()");
}
