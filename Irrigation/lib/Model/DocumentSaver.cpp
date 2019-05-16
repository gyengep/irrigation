#include "DocumentSaver.h"
#include "Configuration.h"
#include "IrrigationDocument.h"
#include "DtoReaderWriter/DtoReaderWriter.h"
#include "Logger/Logger.h"
#include "Utils/FileReaderWriter.h"

using namespace std;


DocumentSaver::DocumentSaver(
		std::shared_ptr<IrrigationDocument> irrigationDocument,
		std::shared_ptr<DtoWriterFactory> dtoWriterFactory,
		std::shared_ptr<FileWriterFactory> fileWriterFactory
	) :
	irrigationDocument(irrigationDocument),
	dtoWriterFactory(dtoWriterFactory),
	fileWriterFactory(fileWriterFactory),
	terminated(false)
{
}

DocumentSaver::~DocumentSaver() {
}

void DocumentSaver::start() {
	unique_lock<mutex> lock(mtx);
	workerThread = thread(&DocumentSaver::workerFunc, this);
}

void DocumentSaver::stop() {
	{
		unique_lock<mutex> lock(mtx);
		terminated = true;
	}

	condition.notify_all();
	workerThread.join();
}

void DocumentSaver::saveIfModified() {
	unique_lock<IrrigationDocument> lock(*irrigationDocument);

	if (irrigationDocument->isModified()) {
		const DocumentDTO documentDto = irrigationDocument->toDocumentDto();
		irrigationDocument->setModified(false);
		lock.unlock();

		try {
			const string documentDtoAsText = dtoWriterFactory->create()->save(documentDto);
			fileWriterFactory->create()->write(documentDtoAsText);
		} catch (...) {
			irrigationDocument->setModified(true);
			throw;
		}
	}
}

void DocumentSaver::load(std::shared_ptr<DtoReader> dtoReader, std::shared_ptr<FileReader> fileReader) {
	const string documentDtoAsText = fileReader->read();
	const DocumentDTO documentDto = dtoReader->loadDocument(documentDtoAsText);

	unique_lock<IrrigationDocument> lock(*irrigationDocument);
	irrigationDocument->updateFromDocumentDto(documentDto);
	irrigationDocument->setModified(false);
}

void DocumentSaver::workerFunc() {
	unique_lock<mutex> lock(mtx);
	while (!terminated) {
		if (condition.wait_for(lock, chrono::seconds(1)) == cv_status::timeout) {
			saveIfModified();
		}
	}
}
