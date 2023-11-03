#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Model/Impl/IrrigationDocumentImpl.h"
#include "Mocks/MockIrrigationDocumentLoader.h"
#include "Mocks/MockIrrigationDocumentSaver.h"
#include "Mocks/MockProgramContainer.h"
#include "Mocks/MockWateringController.h"


class IrrigationDocumentImplTest : public ::testing::Test {
protected:

	std::shared_ptr<IrrigationDocumentImpl> irrigationDocument;

	std::shared_ptr<MockIrrigationDocumentLoader> mockIrrigationDocumentLoader;
	std::shared_ptr<MockIrrigationDocumentSaver> mockIrrigationDocumentSaver;
	std::shared_ptr<MockProgramContainer> mockProgramContainer;
	std::shared_ptr<MockWateringController> mockWateringController;

    virtual void SetUp();
    virtual void TearDown();

public:
    void waitAndUnlock(IrrigationDocumentImpl* irrigationDocument, unsigned waitMs);
};

///////////////////////////////////////////////////////////////////////////////

class IrrigationDocumentImplDtoTest : public IrrigationDocumentImplTest {
protected:

	static const ProgramDtoList sample;

    virtual void SetUp();
    virtual void TearDown();
};
