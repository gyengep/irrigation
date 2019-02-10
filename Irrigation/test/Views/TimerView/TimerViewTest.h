#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Logic/RunTimeContainer.h"
#include "Model/IrrigationDocument.h"
#include "Views/TimerView/TimerView.h"
#include "Mocks/MockProgram.h"
#include "Mocks/MockScheduler.h"
#include "Mocks/MockWateringController.h"

class RunTimeContainer;


class TimerViewTest : public ::testing::Test {
protected:

	static const std::time_t rawtime;
	static const std::tm timeinfo;
	static const unsigned adjustment1 = 70;
	static const unsigned adjustment2 = 80;
	static const RunTimeContainer runTimes1;
	static const RunTimeContainer runTimes2;

	std::shared_ptr<IrrigationDocument> document;
	std::shared_ptr<TimerView> view;
	std::shared_ptr<MockProgram> program1, program2;
	std::shared_ptr<MockScheduler> scheduler1, scheduler2;
	std::shared_ptr<MockWateringController> wateringController;

	virtual void SetUp();
    virtual void TearDown();
};
