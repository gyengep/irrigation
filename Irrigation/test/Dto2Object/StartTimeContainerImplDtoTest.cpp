#include "Logic/StartTimeContainerImpl.h"
#include "Samples/StartTimeContainerSamples.h"
#include "Mocks/MockStartTime.h"
#include <gmock/gmock.h>
#include <memory>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeContainerImplDtoTest, toStartTimeContainerDtoList) {
	const Dto2ObjectTestSamples::StartTimeContainerSampleList sampleList;

	ASSERT_THAT(sampleList, SizeIs(4));

	for (const auto& sample : sampleList) {
		const StartTimeContainerImpl& actual = sample.getContainer();
		const std::list<StartTimeDTO>& expected = sample.getDtoList();

		EXPECT_THAT(actual.toStartTimeDtoList(), ContainerEq(expected));
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeContainerImplDtoTest, createFromStartTimeDto) {
	const Dto2ObjectTestSamples::StartTimeSampleList startTimeSampleList;
	auto mockStartTimeFactory = std::make_shared<StrictMock<MockStartTimeFactory>>(startTimeSampleList.size());

	ASSERT_THAT(startTimeSampleList, SizeIs(4));

	StartTimeContainerImpl startTimeContainer(mockStartTimeFactory);

	EXPECT_CALL(*mockStartTimeFactory, create()).Times(startTimeSampleList.size());

	for (size_t i = 0; i < startTimeSampleList.size(); ++i) {
		const StartTimeDTO& startTimeDto = startTimeSampleList[i].getDto();

		EXPECT_CALL(*mockStartTimeFactory->mockStartTimes[i], updateFromStartTimeDto(startTimeDto)).Times(1);

		auto result = startTimeContainer.createFromStartTimeDto(startTimeDto);

		const auto& id = result.first;

		EXPECT_THAT(startTimeContainer, SizeIs(i + 1));
		EXPECT_THAT(startTimeContainer.at(id), Eq(mockStartTimeFactory->mockStartTimes[i]));
	}
}

///////////////////////////////////////////////////////////////////////////////

void check(std::initializer_list<StartTimeContainer::value_type> initializer) {

	for (const auto& sample : Dto2ObjectTestSamples::StartTimeContainerSampleList()) {
		const std::list<StartTimeDTO>& actualStartTimeDtoList = sample.getDtoList();
		const size_t size = actualStartTimeDtoList.size();

		auto mockStartTimeFactory = std::make_shared<StrictMock<MockStartTimeFactory>>(size);

		EXPECT_CALL(*mockStartTimeFactory, create()).Times(size);

		for (size_t i = 0; i < size; i++) {
			EXPECT_CALL(
					*mockStartTimeFactory->mockStartTimes[i],
					updateFromStartTimeDto(*std::next(actualStartTimeDtoList.begin(), i))
				).Times(1);
		}

		StartTimeContainerImpl actualStartTimeContainer(
			mockStartTimeFactory,
			initializer
		);

		actualStartTimeContainer.updateFromStartTimeDtoList(actualStartTimeDtoList);

		EXPECT_THAT(actualStartTimeContainer, SizeIs(size));

		for (size_t i = 0; i < size; i++) {
			EXPECT_THAT(next(actualStartTimeContainer.begin(), i)->second, Eq(mockStartTimeFactory->mockStartTimes[i]));
		}
	}
}

TEST(StartTimeContainerImplFromDtoTest, updateFromStartTimeContainerDto_zero) {
	std::initializer_list<StartTimeContainer::value_type> initializer {
	};

	check(initializer);
}

TEST(StartTimeContainerImplFromDtoTest, updateFromStartTimeContainerDto_one) {
	std::initializer_list<StartTimeContainer::value_type> initializer {
		{ IdType(), std::make_shared<StrictMock<MockStartTime>>() }
	};

	check(initializer);
}

TEST(StartTimeContainerImplFromDtoTest, updateFromStartTimeContainerDto_more) {
	std::initializer_list<StartTimeContainer::value_type> initializer {
		{ IdType(), std::make_shared<StrictMock<MockStartTime>>() },
		{ IdType(), std::make_shared<StrictMock<MockStartTime>>() }
	};

	check(initializer);
}
