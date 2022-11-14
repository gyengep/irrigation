#pragma once
#include <chrono>
#include <memory>
#include "DocumentView/View.h"
#include "Model/IrrigationDocument.h"
#include "Utils/DateTime.h"
#include "Utils/Thread.h"



class TimerView : public View, public Runnable {
	const std::chrono::seconds period;
	const std::chrono::seconds maxTardiness;
	LocalDateTime expectedLocalDateTime;
	LocalDateTime lastRun;

	IrrigationDocument& irrigationDocument;
	std::unique_ptr<Thread> timerThread;

	static bool checkExpectedDateTime(const LocalDateTime& expectedLocalDateTime);

public:
	TimerView(IrrigationDocument& irrigationDocument);
	virtual ~TimerView();

	virtual void initialize() override;
	virtual void terminate() override;

	virtual void run() override;
};
