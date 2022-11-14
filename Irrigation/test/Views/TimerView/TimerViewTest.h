#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Views/TimerView/TimerView.h"
#include "Mocks/MockTimefunc.h"
#include "Mocks/MockIrrigationDocument.h"


class TimerViewTest : public ::testing::Test {
protected:

	std::shared_ptr<MockTimefunc> mockTimefunc;
	std::shared_ptr<MockIrrigationDocument> mockIrrigationDocument;
	std::shared_ptr<TimerView> view;

	virtual void SetUp();
    virtual void TearDown();
};
