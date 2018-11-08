#pragma once
#include <gmock/gmock.h>
#include <chrono>
#include <memory>
#include "Logic/Program.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/WateringController.h"
#include "Model/IrrigationDocument.h"
#include "Schedulers/Scheduler.h"
#include "Views/TimerView/TimerView.h"

class RunTimeContainer;

///////////////////////////////////////////////////////////////////////////////

class MockScheduler : public Scheduler {
public:
	MOCK_CONST_METHOD1(isDayScheduled, bool(const std::tm& timeinfo));
	MOCK_CONST_METHOD0(getAdjustment, unsigned());
};


class MockProgram : public Program {
public:
	MOCK_CONST_METHOD1(isScheduled, bool(const std::time_t& rawTime));
	MOCK_CONST_METHOD0(getCurrentScheduler, Scheduler&());
};


class MockWateringController : public WateringController {
public:
	MOCK_CONST_METHOD0(isWateringActive, bool());
	MOCK_METHOD1(on1SecTimer, void(const std::time_t&));
	MOCK_METHOD3(start, void(const std::time_t& rawTime, const RunTimeContainer& runTimes, unsigned adjustmentPercent));
	MOCK_METHOD0(stop, void ());
};

///////////////////////////////////////////////////////////////////////////////

class TimerViewTest : public ::testing::Test {
protected:

	static const time_t rawTime;
	static const unsigned adjustment1 = 70;
	static const unsigned adjustment2 = 80;
	static const RunTimeContainer runTimes1;
	static const RunTimeContainer runTimes2;

	std::unique_ptr<IrrigationDocument> document;
	std::unique_ptr<TimerView> view;
	std::unique_ptr<MockProgram> program1, program2;
	std::unique_ptr<MockScheduler> scheduler1, scheduler2;
	std::unique_ptr<MockWateringController> wateringController;

	virtual void SetUp();
    virtual void TearDown();

    void executeTest();
};
