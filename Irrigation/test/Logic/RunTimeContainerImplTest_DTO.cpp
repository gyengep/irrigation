#include "RunTimeContainerImplTest.h"
#include "Exceptions/Exceptions.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const RunTimeDtoList RunTimeContainerImplDtoTest::sample(
	std::list<RunTimeDto> {
		RunTimeDto(10, 20).setId(0),
		RunTimeDto(11, 21).setId(1),
		RunTimeDto(12, 22).setId(2),
		RunTimeDto(13, 23).setId(3),
		RunTimeDto(14, 24).setId(4),
		RunTimeDto(15, 25).setId(5)
	}
);

///////////////////////////////////////////////////////////////////////////////

TEST_F(RunTimeContainerImplDtoTest, toRunTimeContainerDtoList) {
	ASSERT_THAT(mockRunTimeFactory->mockRunTimes, SizeIs(6));

	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[0], toRunTimeDto()).Times(1).WillOnce(Return(*std::next(sample.begin(), 0)));
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[1], toRunTimeDto()).Times(1).WillOnce(Return(*std::next(sample.begin(), 1)));
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[2], toRunTimeDto()).Times(1).WillOnce(Return(*std::next(sample.begin(), 2)));
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[3], toRunTimeDto()).Times(1).WillOnce(Return(*std::next(sample.begin(), 3)));
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[4], toRunTimeDto()).Times(1).WillOnce(Return(*std::next(sample.begin(), 4)));
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[5], toRunTimeDto()).Times(1).WillOnce(Return(*std::next(sample.begin(), 5)));

	EXPECT_THAT(runTimeContainer->toRunTimeDtoList(), ContainerEq(sample));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RunTimeContainerImplDtoTest, updateFromRunTimeDtoList) {
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[0], updateFromRunTimeDto(*std::next(sample.begin(), 0))).Times(1);
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[1], updateFromRunTimeDto(*std::next(sample.begin(), 1))).Times(1);
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[2], updateFromRunTimeDto(*std::next(sample.begin(), 2))).Times(1);
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[3], updateFromRunTimeDto(*std::next(sample.begin(), 3))).Times(1);
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[4], updateFromRunTimeDto(*std::next(sample.begin(), 4))).Times(1);
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[5], updateFromRunTimeDto(*std::next(sample.begin(), 5))).Times(1);

	runTimeContainer->updateFromRunTimeDtoList(sample);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RunTimeContainerImplDtoTest, updateFromRunTimeDtoList_IllegalArgumentException) {
	EXPECT_THROW(runTimeContainer->updateFromRunTimeDtoList(RunTimeDtoList(5)), IllegalArgumentException);
	EXPECT_THROW(runTimeContainer->updateFromRunTimeDtoList(RunTimeDtoList(7)), IllegalArgumentException);
}

TEST_F(RunTimeContainerImplDtoTest, updateFromRunTimeDtoList_ValueOutOfBoundsException) {
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[0], updateFromRunTimeDto(*std::next(sample.begin(), 0))).Times(1);
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[1], updateFromRunTimeDto(*std::next(sample.begin(), 1))).Times(1).WillOnce(Throw(ValueOutOfBoundsException("")));
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[2], updateFromRunTimeDto(*std::next(sample.begin(), 2))).Times(0);
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[3], updateFromRunTimeDto(*std::next(sample.begin(), 3))).Times(0);
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[4], updateFromRunTimeDto(*std::next(sample.begin(), 4))).Times(0);
	EXPECT_CALL(*mockRunTimeFactory->mockRunTimes[5], updateFromRunTimeDto(*std::next(sample.begin(), 5))).Times(0);

	EXPECT_THROW(runTimeContainer->updateFromRunTimeDtoList(sample), ValueOutOfBoundsException);
}
