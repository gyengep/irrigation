#include "Logic/StartTimeContainerImpl.h"
#include "Dto2Object/StartTimeContainerSamples.h"
#include "Mocks/MockStartTime.h"
#include <gmock/gmock.h>
#include <memory>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeContainerImplDtoTest, toStartTimeContainerDtoList) {
	const Dto2ObjectTest::StartTimeContainerSampleList sampleList;

	ASSERT_THAT(sampleList, SizeIs(4));

	for (const auto& sample : sampleList) {
		const StartTimeContainerImpl& actual = sample.getContainer();
		const std::list<StartTimeDTO>& expected = sample.getDtoList();

		EXPECT_THAT(actual.toStartTimeDtoList(), ContainerEq(expected));
	}
}

///////////////////////////////////////////////////////////////////////////////

void check(std::initializer_list<StartTimeContainer::value_type> initializer) {

	for (const auto& sample : Dto2ObjectTest::StartTimeContainerSampleList()) {
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
