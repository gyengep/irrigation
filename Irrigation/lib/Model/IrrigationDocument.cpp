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
		unique_ptr<ProgramContainer>&& programContainer,
		unique_ptr<WateringController>&& wateringController,
		unique_ptr<DtoReaderWriterFactory>&& dtoReaderWriterFactory,
		unique_ptr<FileReaderWriterFactory>&& fileReaderWriterFactory) :
	programContainer(move(programContainer)),
	wateringController(move(wateringController)),
	dtoReaderWriterFactory(move(dtoReaderWriterFactory)),
	fileReaderWriterFactory(move(fileReaderWriterFactory))
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

DocumentDTO IrrigationDocument::getDocumentDTO() const {
	lock_guard<mutex> lock(mtx);

	unique_ptr<list<ProgramDTO>> programDTOs(new list<ProgramDTO>());
	for (auto it = programContainer->begin(); it != programContainer->end(); ++it) {
		programDTOs->push_back(it->second->getProgramDTO().setId(it->first));
	}

	return DocumentDTO(programDTOs.release());
}

void IrrigationDocument::updateFromDTO(const DocumentDTO& documentDTO) {
	lock_guard<mutex> lock(mtx);

	if (documentDTO.hasPrograms()) {
		programContainer.reset(new ProgramContainer());

		for (const ProgramDTO& programDTO : documentDTO.getPrograms()) {
			unique_ptr<Program> program(new Program());
			program->updateFromDTO(programDTO);

			IdType id;
			if (programDTO.hasId()) {
				id = IdType(programDTO.getId());
			}

			programContainer->insert(id, program.release());
		}
	}
}

void IrrigationDocument::load(const string& fileName) {
	LOGGER.debug("Loading configuration...");

	programContainer.reset(new ProgramContainer());

	unique_ptr<FileReader> fileReader(fileReaderWriterFactory->createFileReader());
	const string text = fileReader->read(fileName);

	unique_ptr<DtoReader> dtoReader(dtoReaderWriterFactory->createDtoReader());
	const DocumentDTO documentDTO = dtoReader->loadDocument(text);

	updateFromDTO(documentDTO);

	LOGGER.debug("Configuration is successfully loaded");
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

IrrigationDocument::Builder& IrrigationDocument::Builder::setProgramContainer(unique_ptr<ProgramContainer>&& programContainer) {
	this->programContainer = move(programContainer);
	return *this;
}

IrrigationDocument::Builder& IrrigationDocument::Builder::setWateringController(unique_ptr<WateringController>&& wateringController) {
	this->wateringController = move(wateringController);
	return *this;
}

IrrigationDocument::Builder& IrrigationDocument::Builder::setDtoReaderWriterFactory(unique_ptr<DtoReaderWriterFactory>&& dtoReaderWriterFactory) {
	this->dtoReaderWriterFactory = move(dtoReaderWriterFactory);
	return *this;
}

IrrigationDocument::Builder& IrrigationDocument::Builder::setFileReaderWriterFactory(unique_ptr<FileReaderWriterFactory>&& fileReaderWriterFactory) {
	this->fileReaderWriterFactory = move(fileReaderWriterFactory);
	return *this;
}

unique_ptr<IrrigationDocument> IrrigationDocument::Builder::build() {

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

	return unique_ptr<IrrigationDocument>(new IrrigationDocument(
			move(programContainer),
			move(wateringController),
			move(dtoReaderWriterFactory),
			move(fileReaderWriterFactory)));
}
