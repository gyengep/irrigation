#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Logic/IdType.h"
#include "Logic/RunTimeContainerImpl.h"
#include "Mocks/MockProgram.h"
#include "Mocks/MockScheduler.h"
#include "Mocks/MockWateringController.h"
#include "Model/IrrigationDocumentImpl.h"
#include "Utils/DateTime.h"
#include "Views/TimerView/TimerView.h"


class IrrigationDocumentImplWateringTest : public ::testing::Test {
protected:

	static const LocalDateTime localDateTime;

	static const IdType id1;
	static const IdType id2;

	static const std::string name1;
	static const std::string name2;

	static const unsigned adjustment1 = 70;
	static const unsigned adjustment2 = 80;

	static const unsigned programAdjustment1 = 150;
	static const unsigned programAdjustment2 = 50;

	std::shared_ptr<RunTimeContainer> runTimeContainer1, runTimeContainer2;
	std::shared_ptr<MockProgram> mockProgram1, mockProgram2;
	std::shared_ptr<MockWateringController> mockWateringController;

	std::shared_ptr<IrrigationDocumentImpl> irrigationDocument;

	virtual void SetUp();
    virtual void TearDown();
};
