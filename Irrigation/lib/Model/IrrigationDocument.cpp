#include "IrrigationDocument.h"
#include "DtoReaderWriter/XmlReaderWriterFactory.h"
#include "Logger/Logger.h"
#include "Logic/Program.h"
#include "Logic/RunTime.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTime.h"
#include "Logic/StartTimeContainer.h"
#include "Utils/FileReaderWriterImpl.h"

using namespace std;


IrrigationDocument::IrrigationDocument() :
	IrrigationDocument(
			ZoneHandler::getInstancePtr(),
			new XmlReaderWriterFactory(),
			new FileReaderWriterFactoryImpl())
{
}

IrrigationDocument::IrrigationDocument(std::shared_ptr<ZoneHandler> zoneHandler) :
	IrrigationDocument(
			zoneHandler,
			new XmlReaderWriterFactory(),
			new FileReaderWriterFactoryImpl())
{
}

IrrigationDocument::IrrigationDocument(DtoReaderWriterFactory* readerWriterFactory, FileReaderWriterFactory* fileReaderWriterFactory) :
	IrrigationDocument(
			ZoneHandler::getInstancePtr(),
			readerWriterFactory,
			fileReaderWriterFactory)
{
}

IrrigationDocument::IrrigationDocument(
		shared_ptr<ZoneHandler> zoneHandler,
		DtoReaderWriterFactory* dtoReaderWriterFactory,
		FileReaderWriterFactory* fileReaderWriterFactory) :
	programs(new ProgramContainer()),
	wateringController(zoneHandler),
	dtoReaderWriterFactory(dtoReaderWriterFactory),
	fileReaderWriterFactory(fileReaderWriterFactory)
{
}

IrrigationDocument::~IrrigationDocument() {
}

void IrrigationDocument::lock() const {
	mutex.lock();
}

void IrrigationDocument::unlock() const {
	mutex.unlock();
}

DocumentDTO IrrigationDocument::getDocumentDTO() const {
	lock_guard<std::mutex> lock(mutex);

	unique_ptr<list<ProgramDTO>> programDTOs(new list<ProgramDTO>());
	for (auto it = programs->begin(); it != programs->end(); ++it) {
		programDTOs->push_back(it->second->getProgramDTO().setId(it->first));
	}

	return DocumentDTO(programDTOs.release());
}

void IrrigationDocument::updateFromDTO(const DocumentDTO& documentDTO) {
	lock_guard<std::mutex> lock(mutex);

	if (documentDTO.hasPrograms()) {
		programs.reset(new ProgramContainer());

		for (const ProgramDTO& programDTO : documentDTO.getPrograms()) {
			unique_ptr<Program> program(new Program());
			program->updateFromDTO(programDTO);

			IdType id;
			if (programDTO.hasId()) {
				id = IdType(programDTO.getId());
			}

			programs->insert(id, program.release());
		}
	}
}

void IrrigationDocument::load(const string& fileName) {
	LOGGER.debug("Loading configuration...");

	programs.reset(new ProgramContainer());

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
