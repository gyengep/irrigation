#include "StartTimeImplTest.h"
#include "Exceptions/Exceptions.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void StartTimeImplDtoTest::SetUp() {
	StartTimeImplTest::SetUp();
	startTime->set(originalHours, originalMinutes);
}

void StartTimeImplDtoTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(StartTimeImplDtoTest, toStartTimeDto) {
	const StartTimeDto expectedStartTimeDto(originalHours, originalMinutes);
	EXPECT_THAT(startTime->toStartTimeDto(), Eq(expectedStartTimeDto));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(StartTimeImplDtoTest, updateFromStartTimeDto_empty) {
	StartTimeDto startTimeDto;

	startTime->updateFromStartTimeDto(startTimeDto);

	EXPECT_THAT(startTime->getHours(), Eq(originalHours));
	EXPECT_THAT(startTime->getMinutes(), Eq(originalMinutes));
}

TEST_F(StartTimeImplDtoTest, updateFromStartTimeDto_hours) {
	StartTimeDto startTimeDto;
	startTimeDto.setHours(newHours);

	startTime->updateFromStartTimeDto(startTimeDto);

	EXPECT_THAT(startTime->getHours(), Eq(newHours));
	EXPECT_THAT(startTime->getMinutes(), Eq(0));
}

TEST_F(StartTimeImplDtoTest, updateFromStartTimeDto_minutes) {
	StartTimeDto startTimeDto;
	startTimeDto.setMinutes(newMinutes);

	startTime->updateFromStartTimeDto(startTimeDto);

	EXPECT_THAT(startTime->getHours(), Eq(0));
	EXPECT_THAT(startTime->getMinutes(), Eq(newMinutes));
}

TEST_F(StartTimeImplDtoTest, updateFromStartTimeDto_all) {
	const StartTimeDto startTimeDto(
			newHours,
			newMinutes
		);

	startTime->updateFromStartTimeDto(startTimeDto);

	EXPECT_THAT(startTime->getHours(), Eq(newHours));
	EXPECT_THAT(startTime->getMinutes(), Eq(newMinutes));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(StartTimeImplDtoTest, updateFromStartTimeDto_ValueOutOfBoundsException) {
	const unsigned maxHours = 23;
	const unsigned maxMinutes = 59;

	EXPECT_NO_THROW(startTime->updateFromStartTimeDto(StartTimeDto(maxHours, maxMinutes)));
	EXPECT_THROW(startTime->updateFromStartTimeDto(StartTimeDto(maxHours + 1, maxMinutes)), ValueOutOfBoundsException);
	EXPECT_THROW(startTime->updateFromStartTimeDto(StartTimeDto(maxHours, maxMinutes + 1)), ValueOutOfBoundsException);
}
