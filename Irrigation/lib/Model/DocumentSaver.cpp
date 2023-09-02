#include "DocumentSaver.h"
#include "IrrigationDocument.h"
#include "DtoReaderWriter/DtoReaderWriter.h"
#include "Logger/Logger.h"
#include "Utils/FixedDelaySchedulerRunnable.h"
#include "Utils/ReferenceRunnable.h"

using namespace std;


DocumentSaver::DocumentSaver(
		shared_ptr<IrrigationDocument> irrigationDocument,
		shared_ptr<DtoWriterFactory> dtoWriterFactory,
		shared_ptr<FileWriterFactory> fileWriterFactory
	) :
	irrigationDocument(irrigationDocument),
	dtoWriterFactory(dtoWriterFactory),
	fileWriterFactory(fileWriterFactory)
{
}

DocumentSaver::~DocumentSaver() {
}

void DocumentSaver::startTimer() {
	auto referenceRunnbale = std::make_shared<ReferenceRunnable>(*this);
	auto fixedDelaySchedulerRunnable = std::make_shared<FixedDelaySchedulerRunnable>(
			referenceRunnbale,
			chrono::minutes(1),
			chrono::minutes(1),
			"DocumentSaver"
		);

	timerThread = std::unique_ptr<Thread>(new Thread(fixedDelaySchedulerRunnable, "DocumentSaver"));
	timerThread->start();
}

void DocumentSaver::stopTimer() {
	timerThread->stop();
	timerThread.reset();
}

void DocumentSaver::run() {
#ifdef ONTIMER_TRACE_LOG
	LOGGER.trace("DocumentSaver::onTimer()");
#endif

	try {
		saveIfModified();
	} catch (const exception& e) {
		LOGGER.warning("Can't save configuration", e);
	}
}

void DocumentSaver::saveIfModified() {
	unique_lock<IrrigationDocument> lock(*irrigationDocument);

	if (irrigationDocument->isModified()) {
		const DocumentDTO documentDto = irrigationDocument->toDocumentDto();
		irrigationDocument->setModified(false);
		lock.unlock();

		try {
			const string documentDtoAsText = dtoWriterFactory->create()->save(documentDto);
			fileWriterFactory->create(FileWriter::Type::TRUNCATE)->write(documentDtoAsText);
			LOGGER.debug("Configuration successfully saved");
		} catch (const exception&) {
			irrigationDocument->setModified(true);
			throw;
		}
	}
}

void DocumentSaver::load(const std::shared_ptr<DtoReader>& dtoReader, const std::shared_ptr<FileReader>& fileReader) {
	const string documentDtoAsText = fileReader->read();
	const DocumentDTO documentDto = dtoReader->loadDocument(documentDtoAsText);

	unique_lock<IrrigationDocument> lock(*irrigationDocument);
	irrigationDocument->updateFromDocumentDto(documentDto);
	irrigationDocument->setModified(false);
}
