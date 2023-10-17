#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Model/Impl/IrrigationDocumentImpl.h"
#include "Mocks/MockIrrigationDocumentLoader.h"
#include "Mocks/MockIrrigationDocumentSaver.h"


class IrrigationDocumentImplTest : public ::testing::Test {
protected:

	std::shared_ptr<IrrigationDocumentImpl> irrigationDocument;
	std::shared_ptr<MockIrrigationDocumentLoader> mockIrrigationDocumentLoader;
	std::shared_ptr<MockIrrigationDocumentSaver> mockIrrigationDocumentSaver;

    virtual void SetUp();
    virtual void TearDown();

public:
    void waitAndUnlock(IrrigationDocumentImpl* irrigationDocument, unsigned waitMs);
};
