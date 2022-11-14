#pragma once
#include <chrono>
#include <memory>
#include "DocumentView/View.h"
#include "Model/IrrigationDocument.h"
#include "Utils/DateTime.h"
#include "Utils/Thread.h"



class TimerView : public View, public Runnable {
	const std::chrono::milliseconds period;
	LocalDateTime lastRun;

	IrrigationDocument& irrigationDocument;
	std::unique_ptr<Thread> timerThread;

public:
	TimerView(IrrigationDocument& irrigationDocument);
	virtual ~TimerView();

	virtual void initialize() override;
	virtual void terminate() override;

	virtual void run() override;
};
