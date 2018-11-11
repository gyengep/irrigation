#pragma once
#include <gmock/gmock.h>
#include <memory>

class IrrigationDocument;


class IrrigationDocumentTest : public ::testing::Test {
protected:

	std::unique_ptr<IrrigationDocument> document;

    virtual void SetUp();
    virtual void TearDown();

public:
    void waitAndUnlock(IrrigationDocument* document, unsigned waitMs);
};
