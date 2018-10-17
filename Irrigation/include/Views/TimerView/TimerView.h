#pragma once
#include <chrono>
#include "Timer.h"
#include "DocumentView/View.h"

#if not defined(FRIEND_TEST)
#define FRIEND_TEST(TestCaseName, TestName)
#endif

class IrrigationDocument;


class TimerView : public View, public TimerCallback {
	FRIEND_TEST(TimerViewTest, TestName);

	const std::chrono::seconds deltaT;
	std::chrono::system_clock::time_point expectedSystemTime;

	IrrigationDocument& irrigationDocument;
	Timer timer;

	void onTimer(const time_t rawTime);

	static bool checkSystemTime(const std::chrono::system_clock::time_point& expectedSystemTime);

public:
	TimerView(IrrigationDocument& irrigationDocument);
	virtual ~TimerView();

	virtual void initialize() override;
	virtual void terminate() override;

	virtual void onTimer() override;
};
