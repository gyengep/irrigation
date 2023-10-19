#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Logic/IdType.h"
#include "Mocks/MockProgram.h"
#include "Mocks/MockProgramContainer.h"
#include "Mocks/MockRunTimeContainer.h"
#include "Mocks/MockWateringController.h"
#include "Model/Impl/IrrigationDocumentImpl.h"
#include "Utils/DateTime.h"


class IrrigationDocumentImplWateringTest : public ::testing::Test {
protected:

	static const LocalDateTime localDateTime;

	static const IdType programId1;
	static const IdType programId2;

	static const std::string name1;
	static const std::string name2;

	static const unsigned adjustment1 = 90;
	static const unsigned adjustment2 = 80;

	static const unsigned programAdjustment1 = 70;
	static const unsigned programAdjustment2 = 60;

	static const DurationList durationList1;
	static const DurationList durationList2;

	std::shared_ptr<MockWateringController> mockWateringController;
	std::shared_ptr<MockProgramContainer> mockProgramContainer;
	std::shared_ptr<MockProgram> mockProgram1, mockProgram2;
	std::shared_ptr<MockRunTimeContainer> mockRunTimeContainer1, mockRunTimeContainer2;

	ProgramContainer::container_type programList;

	std::shared_ptr<IrrigationDocumentImpl> irrigationDocument;

	virtual void SetUp();
    virtual void TearDown();
};
