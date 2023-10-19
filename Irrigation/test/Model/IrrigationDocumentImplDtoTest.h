#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Model/Impl/IrrigationDocumentImpl.h"
#include "Mocks/MockProgramContainer.h"


class IrrigationDocumentImplFromDtoTest : public ::testing::Test {
protected:

	enum class UpdateType {
		Nothing,
		ProgramContainer,
		All
	};

	std::shared_ptr<IrrigationDocumentImpl> irrigationDocument;

	std::shared_ptr<MockProgramContainer> mockProgramContainer;

    virtual void SetUp();
    virtual void TearDown();

    void check(const UpdateType updateType);
};
