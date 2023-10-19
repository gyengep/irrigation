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

class StartTimeImplFromDtoTest : public StartTimeImplTest {
protected:

	enum class UpdateType {
		Nothing,
		Hours,
		Minutes,
		All
	};

    void checkUpdateFromStartTimeDto(const UpdateType updateType);
};
