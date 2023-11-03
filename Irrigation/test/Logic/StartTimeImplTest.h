#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Logic/Impl/StartTimeImpl.h"


class StartTimeImplTest : public ::testing::Test {
protected:

	std::shared_ptr<StartTimeImpl> startTime;

    virtual void SetUp();
    virtual void TearDown();
};

///////////////////////////////////////////////////////////////////////////////

class StartTimeImplDtoTest : public StartTimeImplTest {
protected:

	static const unsigned originalHours = 15;
	static const unsigned originalMinutes = 25;

	static const unsigned newHours = 16;
	static const unsigned newMinutes = 46;

    virtual void SetUp();
    virtual void TearDown();
};
