#include "RunTimeContainerImplTest.h"
#include "Exceptions/Exceptions.h"
#include "Dto2ObjectSamples/RunTimeContainerSamples.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeContainerImplToDtoTest, toRunTimeContainerDtoList) {
	const Dto2ObjectTestSamples::RunTimeContainerSampleList sampleList;

	ASSERT_THAT(sampleList, SizeIs(4));

	for (const auto& sample : sampleList) {
		const RunTimeContainerImpl& actual = sample.getContainer();
		const RunTimeDtoList& expected = sample.getDtoList();

		EXPECT_THAT(actual.toRunTimeDtoList(), ContainerEq(expected));
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RunTimeContainerImplFromDtoTest, updateFromRunTimeDtoList) {
	const auto sampleList = Dto2ObjectTestSamples::RunTimeContainerSampleList();

	for (const auto& sample : sampleList) {
		const RunTimeDtoList& actualRunTimeDtoList = sample.getDtoList();

		const size_t expectedSize = actualRunTimeDtoList.size();

		for (size_t i = 0; i < expectedSize; i++) {
			EXPECT_CALL(
					*mockRunTimeFactory->mockRunTimes[i],
					updateFromRunTimeDto(*std::next(actualRunTimeDtoList.begin(), i))
				).Times(1);
		}

		runTimeContainer->updateFromRunTimeDtoList(actualRunTimeDtoList);

		EXPECT_THAT(*runTimeContainer, SizeIs(expectedSize));
	}
}

TEST_F(RunTimeContainerImplFromDtoTest, updateFromRunTimeDtoList_validSize) {
	for (size_t i = 0; i < 6; ++i) {
		EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[i], updateFromRunTimeDto(_));
	}

	EXPECT_NO_THROW(runTimeContainer->updateFromRunTimeDtoList(RunTimeDtoList(6)));
}

TEST_F(RunTimeContainerImplFromDtoTest, updateFromRunTimeDtoList_invalidSize) {
	EXPECT_THROW(runTimeContainer->updateFromRunTimeDtoList(RunTimeDtoList(5)), IllegalArgumentException);
	EXPECT_THROW(runTimeContainer->updateFromRunTimeDtoList(RunTimeDtoList(7)), IllegalArgumentException);
}
