#include "PeriodicDocumentSaver.h"
#include "Logger/Logger.h"
#include "Utils/FixedDelaySchedulerRunnable.h"
#include "Utils/ReferenceRunnable.h"


PeriodicDocumentSaver::PeriodicDocumentSaver(const std::shared_ptr<IrrigationDocument>& irrigationDocument) :
	irrigationDocument(irrigationDocument)
{
}

PeriodicDocumentSaver::~PeriodicDocumentSaver() {
}

void PeriodicDocumentSaver::startTimer() {
	auto referenceRunnbale = std::make_shared<ReferenceRunnable>(*this);
	auto fixedDelaySchedulerRunnable = std::make_shared<FixedDelaySchedulerRunnable>(
			referenceRunnbale,
			std::chrono::minutes(1),
			std::chrono::minutes(1),
			"PeriodicDocumentSaver"
		);

	timerThread = std::unique_ptr<Thread>(new Thread(fixedDelaySchedulerRunnable, "PeriodicDocumentSaver"));
	timerThread->start();
}

void PeriodicDocumentSaver::stopTimer() {
	timerThread->stop();
	timerThread.reset();
}

void PeriodicDocumentSaver::run() {
#ifdef ONTIMER_TRACE_LOG
	LOGGER.trace("PeriodicDocumentSaver::onTimer()");
#endif

	try {
		std::unique_lock<IrrigationDocument> lock(*irrigationDocument);

		if (irrigationDocument->isModified()) {
			irrigationDocument->save();
		}
	} catch (const std::exception& e) {
		LOGGER.warning("Can't save configuration", e);
	}
}
