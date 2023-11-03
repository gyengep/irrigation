#include "RunTimeImplTest.h"
#include "Exceptions/Exceptions.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void RunTimeImplDtoTest::SetUp() {
	RunTimeImplTest::SetUp();
	runTime->set(std::chrono::minutes(originalMinutes) + std::chrono::seconds(originalSeconds));
}

void RunTimeImplDtoTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RunTimeImplDtoTest, toRunTimeDto) {
	const RunTimeDto expectedRunTimeDto(originalMinutes, originalSeconds);
	EXPECT_THAT(runTime->toRunTimeDto(), Eq(expectedRunTimeDto));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RunTimeImplDtoTest, updateFromRunTimeDto_empty) {
	RunTimeDto runTimeDto;

	runTime->updateFromRunTimeDto(runTimeDto);

	EXPECT_THAT(runTime->get(), Eq(std::chrono::minutes(originalMinutes) + std::chrono::seconds(originalSeconds)));
}

TEST_F(RunTimeImplDtoTest, updateFromRunTimeDto_minutes) {
	RunTimeDto runTimeDto;
	runTimeDto.setMinutes(newMinutes);

	runTime->updateFromRunTimeDto(runTimeDto);

	EXPECT_THAT(runTime->get(), Eq(std::chrono::minutes(newMinutes) + std::chrono::seconds(0)));
}

TEST_F(RunTimeImplDtoTest, updateFromRunTimeDto_seconds) {
	RunTimeDto runTimeDto;
	runTimeDto.setSeconds(newSeconds);

	runTime->updateFromRunTimeDto(runTimeDto);

	EXPECT_THAT(runTime->get(), Eq(std::chrono::minutes(0) + std::chrono::seconds(newSeconds)));
}

TEST_F(RunTimeImplDtoTest, updateFromRunTimeDto_all) {
	const RunTimeDto runTimeDto(
			newMinutes,
			newSeconds
		);

	runTime->updateFromRunTimeDto(runTimeDto);

	EXPECT_THAT(runTime->get(), Eq(std::chrono::minutes(newMinutes) + std::chrono::seconds(newSeconds)));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RunTimeImplDtoTest, updateFromRunTimeDto_ValueOutOfBoundsException) {
	const unsigned maxMinutes = 24 * 60;
	const unsigned maxSeconds = 0;

	EXPECT_NO_THROW(runTime->updateFromRunTimeDto(RunTimeDto(maxMinutes, maxSeconds)));
	EXPECT_THROW(runTime->updateFromRunTimeDto(RunTimeDto(maxMinutes + 1, maxSeconds)), ValueOutOfBoundsException);
	EXPECT_THROW(runTime->updateFromRunTimeDto(RunTimeDto(maxMinutes, maxSeconds + 1)), ValueOutOfBoundsException);
}
