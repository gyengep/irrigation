#pragma once
#include <chrono>
#include <memory>
#include "DocumentView/View.h"
#include "Logic/IdType.h"
#include "Logic/Program.h"
#include "Utils/Thread.h"

#ifndef FRIEND_TEST
#define FRIEND_TEST(a, b)
#endif

class IrrigationDocument;
class TimerViewTest;


class TimerView : public View, public Runnable {
	FRIEND_TEST(TimerViewTest, notActiveNotScheduled);
	FRIEND_TEST(TimerViewTest, notActiveScheduledFirst);
	FRIEND_TEST(TimerViewTest, notActiveScheduledSecond);
	FRIEND_TEST(TimerViewTest, active);

	const std::chrono::seconds period;
	const std::chrono::seconds maxTardiness;
	std::chrono::system_clock::time_point expectedSystemTime;

	IrrigationDocument& irrigationDocument;
	std::unique_ptr<Thread> timerThread;

	void checkProgramScheduled(const time_t rawTime);
	bool processProgramScheduled(const IdType& idType, const std::shared_ptr<Program>& program, const time_t rawTime);

	static bool checkSystemTime(const std::chrono::system_clock::time_point& expectedSystemTime);

public:
	TimerView(IrrigationDocument& irrigationDocument);
	virtual ~TimerView();

	virtual void initialize() override;
	virtual void terminate() override;

	virtual void run() override;
};
