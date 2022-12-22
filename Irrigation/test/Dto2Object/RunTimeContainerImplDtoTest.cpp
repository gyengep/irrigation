#include "Logic/RunTimeContainerImpl.h"
#include "Exceptions/Exceptions.h"
#include "Samples/RunTimeContainerSamples.h"
#include "Mocks/MockRunTime.h"
#include <gmock/gmock.h>
#include <memory>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeContainerImplDtoTest, toRunTimeContainerDtoList) {
	const Dto2ObjectTestSamples::RunTimeContainerSampleList sampleList;

	ASSERT_THAT(sampleList, SizeIs(4));

	for (const auto& sample : sampleList) {
		const RunTimeContainerImpl& actual = sample.getContainer();
		const std::list<RunTimeDTO>& expected = sample.getDtoList();

		EXPECT_THAT(actual.toRunTimeDtoList(), ContainerEq(expected));
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeContainerImplFromDtoTest, updateFromRunTimeContainerDto) {

	for (const auto& sample : Dto2ObjectTestSamples::RunTimeContainerSampleList()) {
		const std::list<RunTimeDTO>& actualRunTimeDtoList = sample.getDtoList();
		const size_t size = actualRunTimeDtoList.size();

		auto mockRunTimeFactory = std::make_shared<StrictMock<MockRunTimeFactory>>(size);

		EXPECT_CALL(*mockRunTimeFactory, create()).Times(size);

		for (size_t i = 0; i < size; i++) {
			EXPECT_CALL(
					*mockRunTimeFactory->mockRunTimes[i],
					updateFromRunTimeDto(*std::next(actualRunTimeDtoList.begin(), i))
				).Times(1);
		}

		RunTimeContainerImpl actualRunTimeContainer {
			mockRunTimeFactory
		};

		actualRunTimeContainer.updateFromRunTimeDtoList(actualRunTimeDtoList);

		EXPECT_THAT(actualRunTimeContainer, SizeIs(size));

		for (size_t i = 0; i < size; i++) {
			EXPECT_THAT(next(actualRunTimeContainer.begin(), i)->second, Eq(mockRunTimeFactory->mockRunTimes[i]));
		}
	}
}

TEST(RunTimeContainerImplFromDtoTest, updateFromRunTimeContainerDto_validSize) {

	RunTimeContainerImpl actualRunTimeContainer {
		std::make_shared<NiceMock<MockRunTime>>(),
		std::make_shared<NiceMock<MockRunTime>>(),
		std::make_shared<NiceMock<MockRunTime>>(),
		std::make_shared<NiceMock<MockRunTime>>(),
		std::make_shared<NiceMock<MockRunTime>>(),
		std::make_shared<NiceMock<MockRunTime>>()
	};

	EXPECT_NO_THROW(
			actualRunTimeContainer.updateFromRunTimeDtoList(std::list<RunTimeDTO>(6))
		);
}

TEST(RunTimeContainerImplFromDtoTest, updateFromRunTimeContainerDto_invalidSize) {

	for (size_t i = 0; i < 10; i++) {

		if (6 == i) {
			continue;
		}

		RunTimeContainerImpl actualRunTimeContainer(std::make_shared<MockRunTimeFactory>(6));

		EXPECT_THROW(
				actualRunTimeContainer.updateFromRunTimeDtoList(std::list<RunTimeDTO>(i)),
				IllegalArgumentException
			);
	}
}
