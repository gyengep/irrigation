#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Logic/RunTimeContainer.h"
#include "Model/IrrigationDocument.h"
#include "Views/TimerView/TimerView.h"
#include "MockProgram.h"
#include "MockScheduler.h"
#include "MockWateringController.h"

class RunTimeContainer;



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
