#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Logic/Impl/RunTimeImpl.h"


class RunTimeImplTest : public ::testing::Test {
protected:

	std::shared_ptr<RunTimeImpl> runTime;

    virtual void SetUp();
    virtual void TearDown();
};

///////////////////////////////////////////////////////////////////////////////

class RunTimeImplDtoTest : public RunTimeImplTest {
protected:

	static const unsigned originalMinutes = 2;
	static const unsigned originalSeconds = 15;

	static const unsigned newMinutes = 3;
	static const unsigned newSeconds = 28;

    virtual void SetUp();
    virtual void TearDown();
};
