#pragma once
#include <chrono>
#include <memory>
#include "DocumentView/View.h"
#include "Logic/IdType.h"
#include "Logic/Program.h"
#include "Utils/DateTime.h"
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
	LocalDateTime expectedLocalDateTime;

	IrrigationDocument& irrigationDocument;
	std::unique_ptr<Thread> timerThread;

	void checkProgramScheduled(const LocalDateTime& localDateTime);
	bool processProgramScheduled(const IdType& idType, const std::shared_ptr<Program>& program, const LocalDateTime& localDateTime);

	static bool checkExpectedDateTime(const LocalDateTime& expectedLocalDateTime);

public:
	TimerView(IrrigationDocument& irrigationDocument);
	virtual ~TimerView();

	virtual void initialize() override;
	virtual void terminate() override;

	virtual void run() override;
};
