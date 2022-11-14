#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Model/IrrigationDocumentImpl.h"


class IrrigationDocumentImplTest : public ::testing::Test {
protected:

	std::shared_ptr<IrrigationDocumentImpl> irrigationDocument;

    virtual void SetUp();
    virtual void TearDown();

public:
    void waitAndUnlock(IrrigationDocumentImpl* irrigationDocument, unsigned waitMs);
};
