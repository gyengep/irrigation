#include "IrrigationDocument.h"
#include "Configuration.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTimeContainer.h"
#include "Logic/ProgramContainer.h"
#include "Logic/WateringController.h"
#include <algorithm>
#include <fstream>
#include <iomanip>


#include "Hardware/Valves/GpioValve.h"
#include "Hardware/Valves/ZoneHandlerImpl.h"
#include "Logic/ProgramImpl.h"
#include "Logic/ProgramImplBuilder.h"
#include "Logic/ProgramContainerImpl.h"
#include "Logic/RunTimeContainerImpl.h"
#include "Logic/StartTimeContainerImpl.h"
#include "Schedulers/EveryDaySchedulerImpl.h"
#include "Schedulers/HotWeatherSchedulerImpl.h"
#include "Schedulers/TemperatureDependentSchedulerImpl.h"
#include "Schedulers/WeeklySchedulerImpl.h"
#include "Temperature/TemperatureHandler.h"

using namespace std;


IrrigationDocument::IrrigationDocument(
	const std::shared_ptr<ProgramContainer>& programContainer,
	const std::shared_ptr<ProgramFactory>& programFactory,
	const std::shared_ptr<WateringController>& wateringController
) :
	programs(programContainer),
	programFactory(programFactory),
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

std::pair<IdType, ProgramPtr> IrrigationDocument::createProgram(const ProgramDTO& programDto) {
	ProgramPtr program = programFactory->create();
	program->updateFromProgramDto(programDto);
	return programs->insert(IdType(), program);
}

DocumentDTO IrrigationDocument::toDocumentDto() const {
	return DocumentDTO(programs->toProgramDtoList());
}

void IrrigationDocument::updateFromDocumentDto(const std::shared_ptr<ProgramFactory>& programFactory, const DocumentDTO& documentDTO) {
	if (documentDTO.hasPrograms()) {
		programs->updateFromProgramDtoList(programFactory, documentDTO.getPrograms());
	}

	if (LOGGER.isLoggable(LogLevel::DEBUG)) {
		for (const auto& programWithId : getPrograms()) {
			LOGGER.debug("Program[%s] is added: %s", programWithId.first.toString().c_str(), programWithId.second->toString().c_str());
		}

 		LOGGER.debug("Configuration is successfully loaded");
	}
}

///////////////////////////////////////////////////////////////////////////////

IrrigationDocument::Builder::Builder() {
}

IrrigationDocument::Builder::~Builder() {
}

IrrigationDocument::Builder& IrrigationDocument::Builder::setProgramContainer(const std::shared_ptr<ProgramContainer>& programContainer) {
	this->programContainer = programContainer;
	return *this;
}

IrrigationDocument::Builder& IrrigationDocument::Builder::setProgramFactory(const std::shared_ptr<ProgramFactory>& programFactory) {
	this->programFactory = programFactory;
	return *this;
}

IrrigationDocument::Builder& IrrigationDocument::Builder::setWateringController(const std::shared_ptr<WateringController>& wateringController) {
	this->wateringController = wateringController;
	return *this;
}

shared_ptr<IrrigationDocument> IrrigationDocument::Builder::build() {

	if (nullptr == programContainer) {
		programContainer = std::make_shared<ProgramContainerImpl>();
	}

	if (nullptr == programFactory) {
		programFactory = ProgramImplFactory::Builder().build();
	}

	if (nullptr == wateringController) {
		wateringController = std::make_shared<WateringController>(
				std::make_shared<ZoneHandlerImpl>(GpioValve::getValves())
			);
	}

	return std::make_shared<IrrigationDocument>(
		programContainer,
		programFactory,
		wateringController
	);
}

nlohmann::json IrrigationDocument::saveTo() const {
	nlohmann::json result;

	for (const auto& programWithId : getPrograms()) {
		const string key = "program_" + programWithId.first.toString();
		result[key] = programWithId.second->saveTo();
	}

	return result;
}

void IrrigationDocument::loadFrom(const nlohmann::json& values) {
	for (const auto& programWithId : getPrograms()) {
		const string key = "program_" + programWithId.first.toString();

		auto it = values.find(key);
		if (values.end() != it) {
			programWithId.second->loadFrom(it.value());
		}
	}
}

void IrrigationDocument::saveState() const {
	ofstream o(Configuration::getInstance().getPersistedDatFileName());
	o << setw(4) << saveTo() << endl;
	LOGGER.debug("Document state is saved");
}

void IrrigationDocument::loadState() {
	std::ifstream i(Configuration::getInstance().getPersistedDatFileName());
	if (i.is_open()) {
		nlohmann::json j;
		i >> j;
		loadFrom(j);
		LOGGER.debug("Document state is loaded");
	} else {
		LOGGER.debug("Document state is NOT loaded");
	}
}
