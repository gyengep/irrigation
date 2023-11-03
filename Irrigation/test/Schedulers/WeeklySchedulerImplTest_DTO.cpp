#include "WeeklySchedulerImplTest.h"
#include "Exceptions/Exceptions.h"
#include <list>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

const std::list<bool> WeeklySchedulerImplDtoTest::originalEnabled { true, false, true, true, false, false, false };
const std::list<bool> WeeklySchedulerImplDtoTest::newEnabled { false, true, true, false, false, false, true };

///////////////////////////////////////////////////////////////////////////////

void WeeklySchedulerImplDtoTest::SetUp() {
	WeeklySchedulerImplTest::SetUp();

	if (7 != originalEnabled.size()) {
		throw std::logic_error("7 != originalEnabled.size()");
	}

	if (7 != newEnabled.size()) {
		throw std::logic_error("7 != newEnabled.size()");
	}

	for (size_t i = 0; i < 7; ++i) {
		weeklyScheduler->enableDay(i, *std::next(originalEnabled.begin(), i));
	}
}

void WeeklySchedulerImplDtoTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(WeeklySchedulerImplDtoTest, toWeeklySchedulerDto) {
	const WeeklySchedulerDto expectedWeeklySchedulerDto(
			std::move(std::list<bool>(originalEnabled))
		);

	EXPECT_THAT(weeklyScheduler->toWeeklySchedulerDto(), Eq(expectedWeeklySchedulerDto));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(WeeklySchedulerImplDtoTest, updateFromWeeklySchedulerDto_empty) {
	WeeklySchedulerDto weeklySchedulerDto;

	weeklyScheduler->updateFromWeeklySchedulerDto(weeklySchedulerDto);

	EXPECT_THAT(weeklyScheduler->isDayEnabled(0), *std::next(originalEnabled.begin(), 0));
	EXPECT_THAT(weeklyScheduler->isDayEnabled(1), *std::next(originalEnabled.begin(), 1));
	EXPECT_THAT(weeklyScheduler->isDayEnabled(2), *std::next(originalEnabled.begin(), 2));
	EXPECT_THAT(weeklyScheduler->isDayEnabled(3), *std::next(originalEnabled.begin(), 3));
	EXPECT_THAT(weeklyScheduler->isDayEnabled(4), *std::next(originalEnabled.begin(), 4));
	EXPECT_THAT(weeklyScheduler->isDayEnabled(5), *std::next(originalEnabled.begin(), 5));
	EXPECT_THAT(weeklyScheduler->isDayEnabled(6), *std::next(originalEnabled.begin(), 6));
}

TEST_F(WeeklySchedulerImplDtoTest, updateFromWeeklySchedulerDto_all) {
	const WeeklySchedulerDto weeklySchedulerDto(
			std::move(std::list<bool>(newEnabled))
		);

	weeklyScheduler->updateFromWeeklySchedulerDto(weeklySchedulerDto);

	EXPECT_THAT(weeklyScheduler->isDayEnabled(0), *std::next(newEnabled.begin(), 0));
	EXPECT_THAT(weeklyScheduler->isDayEnabled(1), *std::next(newEnabled.begin(), 1));
	EXPECT_THAT(weeklyScheduler->isDayEnabled(2), *std::next(newEnabled.begin(), 2));
	EXPECT_THAT(weeklyScheduler->isDayEnabled(3), *std::next(newEnabled.begin(), 3));
	EXPECT_THAT(weeklyScheduler->isDayEnabled(4), *std::next(newEnabled.begin(), 4));
	EXPECT_THAT(weeklyScheduler->isDayEnabled(5), *std::next(newEnabled.begin(), 5));
	EXPECT_THAT(weeklyScheduler->isDayEnabled(6), *std::next(newEnabled.begin(), 6));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(WeeklySchedulerImplDtoTest, updateFromWeeklySchedulerDto_IllegalArgumentException) {
	EXPECT_NO_THROW(weeklyScheduler->updateFromWeeklySchedulerDto(std::list<bool>(7)));
	EXPECT_THROW(weeklyScheduler->updateFromWeeklySchedulerDto(WeeklySchedulerDto(std::list<bool>(6))), IllegalArgumentException);
	EXPECT_THROW(weeklyScheduler->updateFromWeeklySchedulerDto(WeeklySchedulerDto(std::list<bool>(8))), IllegalArgumentException);
}
