#pragma once
#include <gmock/gmock.h>
#include <memory>

class IrrigationDocument;


class IrrigationDocumentTest : public ::testing::Test {
protected:

	std::shared_ptr<IrrigationDocument> irrigationDocument;

    virtual void SetUp();
    virtual void TearDown();

public:
    void waitAndUnlock(IrrigationDocument* irrigationDocument, unsigned waitMs);
};
