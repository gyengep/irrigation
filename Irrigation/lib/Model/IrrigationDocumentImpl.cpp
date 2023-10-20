#include "Impl/IrrigationDocumentImpl.h"
#include "Configuration.h"
#include "Email/EmailHandler.h"
#include "Logger/Logger.h"
#include <fstream>
#include <iomanip>


IrrigationDocumentImpl::IrrigationDocumentImpl(
	const std::shared_ptr<Loader>& documentLoader,
	const std::shared_ptr<Saver>& documentSaver,
	const std::shared_ptr<ProgramContainer>& programContainer,
	const std::shared_ptr<WateringController>& wateringController,
	const std::shared_ptr<EmailHandler>& emailHandler
) :
	documentLoader(documentLoader),
	documentSaver(documentSaver),
	programContainer(programContainer),
	wateringController(wateringController),
	emailHandler(emailHandler),
	modified(false)
{
}

IrrigationDocumentImpl::~IrrigationDocumentImpl() {
}

void IrrigationDocumentImpl::lock() const {
	mtx.lock();
}

void IrrigationDocumentImpl::unlock() const {
	mtx.unlock();
}

const ProgramContainer& IrrigationDocumentImpl::getProgramContainer() const {
	return *programContainer;
}

ProgramContainer& IrrigationDocumentImpl::getProgramContainer() {
	return *programContainer;
}

bool IrrigationDocumentImpl::isModified() const {
	return modified;
}

void IrrigationDocumentImpl::setModified(bool modified) {
	this->modified = modified;
}

void IrrigationDocumentImpl::startIfScheduled(const LocalDateTime& localDateTime) {
	if (!wateringController->isWateringActive()) {
		const ProgramContainer& programContainer = getProgramContainer();

		for (const auto& programAndIdPair : programContainer) {
			const IdType id = programAndIdPair.first;
			const ProgramPtr program = programAndIdPair.second;
			const auto schedulerResult = program->isScheduled(localDateTime);

			if (schedulerResult) {
				if (false == schedulerResult->isSkipped()) {
					LOGGER.info("Program[%s] (%s) is started by '%s' scheduler with durations: \n\t%s * %u%% * %u%%",
							id.toString().c_str(),
							program->getName().c_str(),
							to_string(program->getSchedulerType()).c_str(),
							program->getRunTimeContainer().toDurationList().toString().c_str(),
							program->getAdjustment(),
							schedulerResult->getAdjustment()
						);

					const EmailTopic topic = EmailTopic::WATERING_START;
					if (emailHandler && emailHandler->isTopicEnabled(topic)) {
						std::ostringstream oss;
						oss << "The " << program->getName() << " is started by the '" << to_string(program->getSchedulerType()) << "' scheduler at " << localDateTime.toString("%T") << std::endl;
						oss << "adjustment: "<< schedulerResult->getAdjustment() << "%" << std::endl;
						oss << "runTimes:   " << program->getRunTimeContainer() << std::endl;
						emailHandler->send(topic, oss.str());
					}


					const DurationList durations = program->getRunTimeContainer().toDurationList().adjust(program->getAdjustment());
					start(durations, schedulerResult->getAdjustment());
					saveState();
					break;
				} else {
					LOGGER.debug("Program[%s] (%s) is skipped by '%s' scheduler",
							id.toString().c_str(),
							program->getName().c_str(),
							to_string(program->getSchedulerType()).c_str()
						);

					const EmailTopic topic = EmailTopic::WATERING_SKIP;
					if (emailHandler && emailHandler->isTopicEnabled(topic)) {
						std::ostringstream oss;
						oss << "The " << program->getName() << " is skipped by the " << to_string(program->getSchedulerType()) << " scheduler at " << localDateTime.toString("%T") << std::endl;
						emailHandler->send(topic, oss.str());
					}
				}
			}
		}
	}
}

void IrrigationDocumentImpl::startProgram(const IdType& programId, unsigned adjustment) {
	const ProgramPtr program = programContainer->at(programId);
	const DurationList durations = program->getRunTimeContainer().toDurationList();

	LOGGER.info("Program[%s] (%s) is started by user with durations: \n\t%s * %u%%",
			programId.toString().c_str(),
			program->getName().c_str(),
			durations.toString().c_str(),
			adjustment
		);

	start(durations, adjustment);
}

void IrrigationDocumentImpl::startCustom(const DurationList& durations, unsigned adjustment) {
	LOGGER.info("A custom program is started by user with durations: \n\t%s * %u%%",
			durations.toString().c_str(),
			adjustment
		);
	start(durations, adjustment);
}

void IrrigationDocumentImpl::start(const DurationList& durations, unsigned adjustment) {
	wateringController->start(durations.adjust(adjustment));
}

void IrrigationDocumentImpl::stop() {
	LOGGER.info("Irrigation is stopped by user");
	wateringController->stop();
}

DocumentDto IrrigationDocumentImpl::toDocumentDto() const {
	return DocumentDto(programContainer->toProgramDtoList());
}

void IrrigationDocumentImpl::updateFromDocumentDto(const DocumentDto& documentDto) {
	if (documentDto.hasPrograms()) {
		programContainer->updateFromProgramDtoList(documentDto.getPrograms());
	}

	if (LOGGER.isLoggable(LogLevel::DEBUG)) {
		for (const auto& programWithId : getProgramContainer()) {
			LOGGER.debug("Program[%s] is added: %s", programWithId.first.toString().c_str(), programWithId.second->toString().c_str());
		}
	}
}

nlohmann::json IrrigationDocumentImpl::saveTo() const {
	nlohmann::json result;

	for (const auto& programWithId : getProgramContainer()) {
		const std::string key = "program_" + programWithId.first.toString();
		result[key] = programWithId.second->saveTo();
	}

	return result;
}

void IrrigationDocumentImpl::loadFrom(const nlohmann::json& values) {
	for (const auto& programWithId : getProgramContainer()) {
		const std::string key = "program_" + programWithId.first.toString();

		auto it = values.find(key);
		if (values.end() != it) {
			programWithId.second->loadFrom(it.value());
		}
	}
}

void IrrigationDocumentImpl::saveState() const {
	std::ofstream of(Configuration::getInstance().getPersistedDatFileName());
	of << std::setw(4) << saveTo() << std::endl;
	LOGGER.debug("Document state is saved");
}

void IrrigationDocumentImpl::loadState() {
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

void IrrigationDocumentImpl::save() const {
	documentSaver->save(*this);
	modified = false;
}

void IrrigationDocumentImpl::load() {
	modified = (false == documentLoader->load(*this));
}
