#pragma once
#include <memory>
#include "Utils/Runnable.h"
#include "Utils/Thread.h"
#include "IrrigationDocument.h"



class PeriodicDocumentSaver : public Runnable {
	const std::shared_ptr<IrrigationDocument> irrigationDocument;

	std::unique_ptr<Thread> timerThread;

public:
	PeriodicDocumentSaver(const std::shared_ptr<IrrigationDocument>& irrigationDocument);
	virtual ~PeriodicDocumentSaver();

	void startTimer();
	void stopTimer();

	virtual void run() override;
};
