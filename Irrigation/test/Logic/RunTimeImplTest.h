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

class RunTimeImplFromDtoTest : public RunTimeImplTest {
protected:

	enum class UpdateType {
		Nothing,
		Minutes,
		Seconds,
		All
	};

    void checkUpdateFromRunTimeDto(const UpdateType updateType);
};
