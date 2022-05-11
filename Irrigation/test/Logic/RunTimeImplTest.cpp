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

TEST(RunTimeImplTest, equalsOperator) {
	EXPECT_TRUE(RunTimeImpl(10) == RunTimeImpl(10));
	EXPECT_FALSE(RunTimeImpl(10) == RunTimeImpl(11));
	EXPECT_FALSE(RunTimeImpl(11) == RunTimeImpl(10));
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

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeImplTest, toRunTimeDto) {
	RunTimeImpl runTime;
	RunTimeDTO expected;

	runTime.setSeconds(0);
	EXPECT_THAT(runTime.toRunTimeDto(), Eq(RunTimeDTO(0, 0)));

	runTime.setSeconds(39);
	EXPECT_THAT(runTime.toRunTimeDto(), Eq(RunTimeDTO(0, 39)));

	runTime.setSeconds(18);
	EXPECT_THAT(runTime.toRunTimeDto(), Eq(RunTimeDTO(0, 18)));

	runTime.setSeconds(75);
	EXPECT_THAT(runTime.toRunTimeDto(), Eq(RunTimeDTO(1, 15)));

	runTime.setSeconds(150);
	EXPECT_THAT(runTime.toRunTimeDto(), Eq(RunTimeDTO(2, 30)));
}

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeImplTest, partialUpdateFromRunTimeDto_empty) {
	const RunTimeImpl expected(25);

	RunTimeImpl actual(25);

	actual.updateFromRunTimeDto(RunTimeDTO());
	EXPECT_THAT(actual, Eq(std::ref(expected)));
}

TEST(RunTimeImplTest, partialUpdateFromRunTimeDto_seconds) {
	const RunTimeImpl expected1(39);
	const RunTimeImpl expected2(97);

	RunTimeImpl actual;

	actual.updateFromRunTimeDto(RunTimeDTO().setSeconds(39));
	EXPECT_THAT(actual, Eq(std::ref(expected1)));

	actual.updateFromRunTimeDto(RunTimeDTO().setSeconds(97));
	EXPECT_THAT(actual, Eq(std::ref(expected2)));
}

TEST(RunTimeImplTest, partialUpdateFromRunTimeDto_minutes) {
	const RunTimeImpl expected1(600);
	const RunTimeImpl expected2(900);

	RunTimeImpl actual;

	actual.updateFromRunTimeDto(RunTimeDTO().setMinutes(10));
	EXPECT_THAT(actual, Eq(std::ref(expected1)));

	actual.updateFromRunTimeDto(RunTimeDTO().setMinutes(15));
	EXPECT_THAT(actual, Eq(std::ref(expected2)));
}

TEST(RunTimeImplTest, partialUpdateFromRunTimeDto_all) {
	const RunTimeImpl expected1(605);
	const RunTimeImpl expected2(910);

	RunTimeImpl actual;

	actual.updateFromRunTimeDto(RunTimeDTO().setMinutes(10).setSeconds(5));
	EXPECT_THAT(actual, Eq(std::ref(expected1)));

	actual.updateFromRunTimeDto(RunTimeDTO().setMinutes(15).setSeconds(10));
	EXPECT_THAT(actual, Eq(std::ref(expected2)));
}

