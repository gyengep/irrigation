#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Model/IrrigationDocument.h"



class IrrigationDocumentTest : public ::testing::Test {
protected:

	std::unique_ptr<IrrigationDocument> document;

    virtual void SetUp();
    virtual void TearDown();

public:
    void waitAndUnlock(IrrigationDocument* document, unsigned waitMs);
};
