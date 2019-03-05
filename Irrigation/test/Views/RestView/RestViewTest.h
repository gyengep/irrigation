#pragma once
#include <memory>
#include <gmock/gmock.h>

class IrrigationDocument;
class RestView;


class RestViewTest : public  testing::Test {
protected:

	std::shared_ptr<IrrigationDocument> document;

    virtual void SetUp();
    virtual void TearDown();
};
