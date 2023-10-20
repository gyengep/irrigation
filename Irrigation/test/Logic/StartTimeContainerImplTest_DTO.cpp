#include "StartTimeContainerImplTest.h"
#include "Dto2ObjectSamples/StartTimeContainerSamples.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeContainerImplToDtoTest, toStartTimeContainerDtoList) {
	const Dto2ObjectTestSamples::StartTimeContainerSampleList sampleList;

	ASSERT_THAT(sampleList, SizeIs(4));

	for (const auto& sample : sampleList) {
		const StartTimeContainerImpl& actual = sample.getContainer();
		const StartTimeDtoList& expected = sample.getDtoList();

		EXPECT_THAT(actual.toStartTimeDtoList(), ContainerEq(expected));
	}
}

///////////////////////////////////////////////////////////////////////////////

void StartTimeContainerImplFromDtoTest::checkUpdateFromStartTimeDtoList() {
	const auto sampleList = Dto2ObjectTestSamples::StartTimeContainerSampleList();

	unsigned totalCount = 0;
	for (const auto& sample : sampleList) {
		totalCount += sample.getDtoList().size();
	}

	EXPECT_CALL(*mockStartTimeFactory, create()).
			Times(totalCount).
			WillRepeatedly(Invoke(mockStartTimeFactory.get(), &MockStartTimeFactory::createMockStartTime));

	for (const auto& sample : sampleList) {
		const size_t expectedSize = sample.getDtoList().size();

		for (const StartTimeDto& startTimeDto : sample.getDtoList()) {
			mockStartTimeFactory->mockStartTimes.push_back(std::make_shared<StrictMock<MockStartTime>>());
			EXPECT_CALL(*mockStartTimeFactory->mockStartTimes.back(), updateFromStartTimeDto(startTimeDto)).
					Times(1);
		}

		startTimeContainer->updateFromStartTimeDtoList(sample.getDtoList());

		EXPECT_THAT(*startTimeContainer, SizeIs(expectedSize));

		for (size_t i = 0; i < expectedSize; i++) {
			EXPECT_THAT(
					std::next(startTimeContainer->begin(), i)->second,
					Eq(mockStartTimeFactory->mockStartTimes[mockStartTimeFactory->mockStartTimes.size() - expectedSize + i])
				);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(StartTimeContainerImplFromDtoTest, updateFromStartTimeDtoList_zero) {
	ASSERT_THAT(*startTimeContainer, SizeIs(0));
	checkUpdateFromStartTimeDtoList();
}

TEST_F(StartTimeContainerImplFromDtoTest, updateFromStartTimeDtoList_one) {
	startTimeContainer->insert(IdType(), std::make_shared<StrictMock<MockStartTime>>());

	ASSERT_THAT(*startTimeContainer, SizeIs(1));
	checkUpdateFromStartTimeDtoList();
}

TEST_F(StartTimeContainerImplFromDtoTest, updateFromStartTimeDtoList_more) {
	startTimeContainer->insert(IdType(), std::make_shared<StrictMock<MockStartTime>>());
	startTimeContainer->insert(IdType(), std::make_shared<StrictMock<MockStartTime>>());

	ASSERT_THAT(*startTimeContainer, SizeIs(2));
	checkUpdateFromStartTimeDtoList();
}
