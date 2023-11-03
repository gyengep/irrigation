#pragma once
#include <gmock/gmock.h>
#include <memory>
#include "Logic/Impl/StartTimeContainerImpl.h"
#include "Mocks/MockStartTime.h"


class StartTimeContainerImplTest : public ::testing::Test {
protected:

	std::shared_ptr<StartTimeContainerImpl> startTimeContainer;
	std::shared_ptr<MockStartTimeFactory> mockStartTimeFactory;

    virtual void SetUp();
    virtual void TearDown();
};

///////////////////////////////////////////////////////////////////////////////

class StartTimeContainerImplDtoTest : public StartTimeContainerImplTest {
protected:

	static const StartTimeDtoList sampleList_ZeroItem;
	static const StartTimeDtoList sampleList_OneItem;
	static const StartTimeDtoList sampleList_MoreItem;

	void checkToStartTimeDtoList(const StartTimeDtoList& sampleList);
	void checkFromStartTimeDtoList(const StartTimeDtoList& sampleList);

	void checkFromStartTimeDtoList_zeroItem();
	void checkFromStartTimeDtoList_oneItem();
	void checkFromStartTimeDtoList_moreItem();
};
