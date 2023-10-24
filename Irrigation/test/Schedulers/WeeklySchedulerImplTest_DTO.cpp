#include "WeeklySchedulerImplTest.h"
#include "Exceptions/Exceptions.h"
#include "Dto2ObjectSamples/WeeklySchedulerSamples.h"
#include <list>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(WeeklySchedulerImplToDtoTest, toWeeklySchedulerDto) {
	const Dto2ObjectTestSamples::WeeklySchedulerSampleList sampleList;

	ASSERT_THAT(sampleList, SizeIs(4));

	for (const auto& sample : sampleList) {
		const WeeklySchedulerImpl& actual = sample.getObject();
		const WeeklySchedulerDto& expected = sample.getDto();

		EXPECT_THAT(actual.toWeeklySchedulerDto(), Eq(expected));
	}
}

///////////////////////////////////////////////////////////////////////////////

void WeeklySchedulerImplFromDtoTest::checkUpdateFromWeeklySchedulerDto(const UpdateType updateType) {
	const auto sampleList = Dto2ObjectTestSamples::WeeklySchedulerSampleList();

	for (const auto& sample : sampleList) {
		WeeklySchedulerDto actualWeeklySchedulerDto;

		std::list<bool> expected {
			weeklyScheduler->isDayEnabled(0),
			weeklyScheduler->isDayEnabled(1),
			weeklyScheduler->isDayEnabled(2),
			weeklyScheduler->isDayEnabled(3),
			weeklyScheduler->isDayEnabled(4),
			weeklyScheduler->isDayEnabled(5),
			weeklyScheduler->isDayEnabled(6)
		};

		ASSERT_THAT(expected.size(), Eq(WeeklySchedulerImpl::count));

		if (UpdateType::Days == updateType || UpdateType::All == updateType) {
			expected = sample.getDto().getValues();
			actualWeeklySchedulerDto.setValues(std::list<bool>(expected));
		}

		weeklyScheduler->updateFromWeeklySchedulerDto(actualWeeklySchedulerDto);

		EXPECT_THAT(weeklyScheduler->isDayEnabled(0), Eq(*std::next(expected.begin(), 0)));
		EXPECT_THAT(weeklyScheduler->isDayEnabled(1), Eq(*std::next(expected.begin(), 1)));
		EXPECT_THAT(weeklyScheduler->isDayEnabled(2), Eq(*std::next(expected.begin(), 2)));
		EXPECT_THAT(weeklyScheduler->isDayEnabled(3), Eq(*std::next(expected.begin(), 3)));
		EXPECT_THAT(weeklyScheduler->isDayEnabled(4), Eq(*std::next(expected.begin(), 4)));
		EXPECT_THAT(weeklyScheduler->isDayEnabled(5), Eq(*std::next(expected.begin(), 5)));
		EXPECT_THAT(weeklyScheduler->isDayEnabled(6), Eq(*std::next(expected.begin(), 6)));
	}
}

TEST_F(WeeklySchedulerImplFromDtoTest, updateFromWeeklySchedulerDto_empty) {
	weeklyScheduler->enableDay(0, true);
	weeklyScheduler->enableDay(1, false);
	weeklyScheduler->enableDay(2, false);
	weeklyScheduler->enableDay(3, false);
	weeklyScheduler->enableDay(4, false);
	weeklyScheduler->enableDay(5, false);
	weeklyScheduler->enableDay(6, true);

	checkUpdateFromWeeklySchedulerDto(UpdateType::Nothing);
}

TEST_F(WeeklySchedulerImplFromDtoTest, updateFromWeeklySchedulerDto_partial_days) {
	weeklyScheduler->enableDay(0, false);
	weeklyScheduler->enableDay(1, true);
	weeklyScheduler->enableDay(2, true);
	weeklyScheduler->enableDay(3, true);
	weeklyScheduler->enableDay(4, true);
	weeklyScheduler->enableDay(5, true);
	weeklyScheduler->enableDay(6, false);

	checkUpdateFromWeeklySchedulerDto(UpdateType::Days);
}

TEST_F(WeeklySchedulerImplFromDtoTest, updateFromWeeklySchedulerDto_all) {
	weeklyScheduler->enableDay(0, false);
	weeklyScheduler->enableDay(1, false);
	weeklyScheduler->enableDay(2, true);
	weeklyScheduler->enableDay(3, true);
	weeklyScheduler->enableDay(4, true);
	weeklyScheduler->enableDay(5, false);
	weeklyScheduler->enableDay(6, false);

	checkUpdateFromWeeklySchedulerDto(UpdateType::All);
}

TEST_F(WeeklySchedulerImplFromDtoTest, updateFromWeeklySchedulerDto_validSize) {
	EXPECT_NO_THROW(weeklyScheduler->updateFromWeeklySchedulerDto(std::list<bool>(7)));
}

TEST_F(WeeklySchedulerImplFromDtoTest, updateFromWeeklySchedulerDto_invalidSize) {
	EXPECT_THROW(weeklyScheduler->updateFromWeeklySchedulerDto(WeeklySchedulerDto(std::list<bool>(6))), IllegalArgumentException);
	EXPECT_THROW(weeklyScheduler->updateFromWeeklySchedulerDto(WeeklySchedulerDto(std::list<bool>(8))), IllegalArgumentException);
}
