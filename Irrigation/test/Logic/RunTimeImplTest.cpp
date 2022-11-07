#include <gmock/gmock.h>
#include "Exceptions/Exceptions.h"
#include "Logic/RunTimeImpl.h"

using namespace std;
using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeImplTest, defaultConstructor) {
	RunTimeImpl runTime;
	EXPECT_THAT(runTime.getSeconds(), Eq(0));
}

TEST(RunTimeImplTest, parametrizedConstructor) {
	RunTimeImpl runTime(10);
	EXPECT_THAT(runTime.getSeconds(), Eq(10));
}

TEST(RunTimeImplTest, setValue) {
	RunTimeImpl runTime;

	runTime.setSeconds(25);
	EXPECT_THAT(runTime.getSeconds(), Eq(25));

	runTime.setSeconds(38);
	EXPECT_THAT(runTime.getSeconds(), Eq(38));
}

TEST(RunTimeImplTest, setValueMax) {
	RunTimeImpl runTime;

	EXPECT_NO_THROW(runTime.setSeconds(24 * 60 * 60));
	EXPECT_THROW(runTime.setSeconds(24 * 60 * 60 + 1), ValueOutOfBoundsException);
}

TEST(RunTimeImplTest, toDuration) {
	EXPECT_THAT(RunTimeImpl(0).toDuration(), Eq(std::chrono::seconds(0)));
	EXPECT_THAT(RunTimeImpl(12).toDuration(), Eq(std::chrono::seconds(12)));
	EXPECT_THAT(RunTimeImpl(85).toDuration(), Eq(std::chrono::seconds(85)));
}

TEST(RunTimeImplTest, toDurationStatic) {
	EXPECT_THAT(RunTime::toDuration(RunTimeDTO(0, 0)), Eq(std::chrono::seconds(0)));
	EXPECT_THAT(RunTime::toDuration(RunTimeDTO(0, 12)), Eq(std::chrono::seconds(12)));
	EXPECT_THAT(RunTime::toDuration(RunTimeDTO(1, 25)), Eq(std::chrono::seconds(85)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeImplTest, toRunTimeDto) {
	EXPECT_THAT(RunTimeImpl(0).toRunTimeDto(), Eq(RunTimeDTO(0, 0)));
	EXPECT_THAT(RunTimeImpl(39).toRunTimeDto(), Eq(RunTimeDTO(0, 39)));
	EXPECT_THAT(RunTimeImpl(18).toRunTimeDto(), Eq(RunTimeDTO(0, 18)));
	EXPECT_THAT(RunTimeImpl(75).toRunTimeDto(), Eq(RunTimeDTO(1, 15)));
	EXPECT_THAT(RunTimeImpl(150).toRunTimeDto(), Eq(RunTimeDTO(2, 30)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeImplTest, updateFromDto_empty) {
	const unsigned expectedSeconds1 = 25;

	RunTimeImpl actual(expectedSeconds1);
	actual.updateFromRunTimeDto(RunTimeDTO());

	EXPECT_THAT(actual.getSeconds(), Eq(expectedSeconds1));
}

TEST(RunTimeImplTest, updateFromDto_partial_seconds) {
	const unsigned expectedSeconds1 = 39;
	const unsigned expectedSeconds2 = 97;

	RunTimeImpl actual;

	actual.updateFromRunTimeDto(RunTimeDTO().setSeconds(expectedSeconds1));
	EXPECT_THAT(actual.getSeconds(), Eq(expectedSeconds1));

	actual.updateFromRunTimeDto(RunTimeDTO().setSeconds(expectedSeconds2));
	EXPECT_THAT(actual.getSeconds(), Eq(expectedSeconds2));
}

TEST(RunTimeImplTest, updateFromDto_partial_minutes) {
	const unsigned expectedMinutes1 = 10;
	const unsigned expectedMinutes2 = 15;

	RunTimeImpl actual;

	actual.updateFromRunTimeDto(
		RunTimeDTO().
			setMinutes(expectedMinutes1)
	);

	EXPECT_THAT(actual.getSeconds(), Eq(expectedMinutes1 * 60));

	actual.updateFromRunTimeDto(
		RunTimeDTO().
			setMinutes(expectedMinutes2)
	);

	EXPECT_THAT(actual.getSeconds(), Eq(expectedMinutes2 * 60));
}

TEST(RunTimeImplTest, updateFromDto_all) {
	const unsigned expectedMinutes1 = 10;
	const unsigned expectedSeconds1 = 5;
	const unsigned expectedMinutes2 = 15;
	const unsigned expectedSeconds2 = 10;

	RunTimeImpl actual;

	actual.updateFromRunTimeDto(
		RunTimeDTO().
			setMinutes(expectedMinutes1).
			setSeconds(expectedSeconds1)
	);

	EXPECT_THAT(actual.getSeconds(), Eq(expectedMinutes1 * 60 + expectedSeconds1));

	actual.updateFromRunTimeDto(
		RunTimeDTO().
			setMinutes(expectedMinutes2).
			setSeconds(expectedSeconds2)
	);

	EXPECT_THAT(actual.getSeconds(), Eq(expectedMinutes2 * 60 + expectedSeconds2));
}
