#include "IrrigationDocument.h"
#include "DtoReaderWriter/XmlReaderWriterFactory.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"
#include "Logic/RunTime.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTime.h"
#include "Logic/StartTimeContainer.h"
#include "Logic/ProgramContainer.h"
#include "Logic/WateringController.h"
#include "Utils/FileReaderWriterImpl.h"

using namespace std;


IrrigationDocument::IrrigationDocument(
		shared_ptr<ProgramContainer> programContainer,
		shared_ptr<WateringController> wateringController,
		shared_ptr<DtoReaderWriterFactory> dtoReaderWriterFactory,
		shared_ptr<FileReaderWriterFactory> fileReaderWriterFactory) :
	programs(programContainer),
	wateringController(wateringController),
	dtoReaderWriterFactory(dtoReaderWriterFactory),
	fileReaderWriterFactory(fileReaderWriterFactory)
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
	lock_guard<mutex> lock(mtx);

	return DocumentDTO(programs->toProgramDtoList());
}

void IrrigationDocument::updateFromDocumentDto(const DocumentDTO& documentDTO) {
	lock_guard<mutex> lock(mtx);

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

void IrrigationDocument::load(const string& fileName) {
	LOGGER.debug("Loading configuration...");

	unique_ptr<FileReader> fileReader = fileReaderWriterFactory->createFileReader();
	unique_ptr<DtoReader> dtoReader = dtoReaderWriterFactory->createDtoReader();

	const string text = fileReader->read(fileName);
	const DocumentDTO documentDTO = dtoReader->loadDocument(text);

	updateFromDocumentDto(documentDTO);
}

void IrrigationDocument::save(const string& fileName) const {
	throw logic_error("Method not implemented: IrrigationDocument::save()");

	//	const DocumentDTO documentDTO = getDocumentDTO();
	//	const string xml = XmlWriter().save(documentDTO);
	//
	//	try {
	//		writeFile(fileName, xml);
	//		LOGGER.debug("Configuration successfully saved.");
	//	} catch (const IOException& e) {
	//		LOGGER.warning("Configuration file saving failed. %s", e.what());
	//	}
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

IrrigationDocument::Builder& IrrigationDocument::Builder::setDtoReaderWriterFactory(shared_ptr<DtoReaderWriterFactory> dtoReaderWriterFactory) {
	this->dtoReaderWriterFactory = dtoReaderWriterFactory;
	return *this;
}

IrrigationDocument::Builder& IrrigationDocument::Builder::setFileReaderWriterFactory(shared_ptr<FileReaderWriterFactory> fileReaderWriterFactory) {
	this->fileReaderWriterFactory = fileReaderWriterFactory;
	return *this;
}

shared_ptr<IrrigationDocument> IrrigationDocument::Builder::build() {

	if (nullptr == programContainer) {
		programContainer.reset(new ProgramContainer());
	}

	if (nullptr == wateringController) {
		wateringController.reset(new WateringController());
	}

	if (nullptr == dtoReaderWriterFactory) {
		dtoReaderWriterFactory.reset(new XmlReaderWriterFactory());
	}

	if (nullptr == fileReaderWriterFactory) {
		fileReaderWriterFactory.reset(new FileReaderWriterFactoryImpl());
	}

	return shared_ptr<IrrigationDocument>(new IrrigationDocument(
			programContainer,
			wateringController,
			dtoReaderWriterFactory,
			fileReaderWriterFactory));
}
