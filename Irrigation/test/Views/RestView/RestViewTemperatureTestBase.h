#pragma once
#include <memory>
#include <gmock/gmock.h>
#include "Mocks/MockTimefunc.h"
#include "RestViewTest.h"


class RestViewTemperatureTestBase : public RestViewTest {
protected:

	std::shared_ptr<MockTimefunc> mockTimefunc;

    virtual void SetUp();
    virtual void TearDown();
};
