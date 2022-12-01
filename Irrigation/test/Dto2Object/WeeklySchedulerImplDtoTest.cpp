#include "Schedulers/WeeklySchedulerImpl.h"
#include "Exceptions/Exceptions.h"
#include "Dto2Object/WeeklySchedulerSamples.h"
#include <gmock/gmock.h>
#include <list>
#include <memory>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(WeeklySchedulerImplToDtoTest, toWeeklySchedulerDto) {
	const Dto2ObjectTest::WeeklySchedulerSampleList sampleList;

	ASSERT_THAT(sampleList, SizeIs(4));

	for (const auto& sample : sampleList) {
		const WeeklySchedulerImpl& actual = sample.getObject();
		const WeeklySchedulerDTO& expected = sample.getDto();

		EXPECT_THAT(actual.toWeeklySchedulerDto(), Eq(expected));
	}
}

///////////////////////////////////////////////////////////////////////////////

enum class WeeklySchedulerDtoUpdateType {
	Nothing,
	Days,
	All
};

std::vector<bool> toDays(const WeeklySchedulerImpl& weeklyScheduler) {
	std::vector<bool> result(7);

	for (size_t i = 0; i < 7; i++) {
		result[i] = weeklyScheduler.isDayEnabled(i);
	}

	return result;
}

void check(const WeeklySchedulerDtoUpdateType updateType, const std::vector<bool>& days) {

	WeeklySchedulerImpl actualWeeklyScheduler {
		days[0],
		days[1],
		days[2],
		days[3],
		days[4],
		days[5],
		days[6]
	};

	std::vector<bool> expected = days;

	for (const auto& sample : Dto2ObjectTest::WeeklySchedulerSampleList()) {
		WeeklySchedulerDTO actualWeeklySchedulerDTO;

		if (WeeklySchedulerDtoUpdateType::Days == updateType || WeeklySchedulerDtoUpdateType::All == updateType) {
			expected = toDays(sample.getObject());
			actualWeeklySchedulerDTO.setValues(std::list<bool>(expected.begin(), expected.end()));
		}

		actualWeeklyScheduler.updateFromWeeklySchedulerDto(actualWeeklySchedulerDTO);

		EXPECT_THAT(actualWeeklyScheduler.isDayEnabled(0), Eq(expected[0]));
		EXPECT_THAT(actualWeeklyScheduler.isDayEnabled(1), Eq(expected[1]));
		EXPECT_THAT(actualWeeklyScheduler.isDayEnabled(2), Eq(expected[2]));
		EXPECT_THAT(actualWeeklyScheduler.isDayEnabled(3), Eq(expected[3]));
		EXPECT_THAT(actualWeeklyScheduler.isDayEnabled(4), Eq(expected[4]));
		EXPECT_THAT(actualWeeklyScheduler.isDayEnabled(5), Eq(expected[5]));
		EXPECT_THAT(actualWeeklyScheduler.isDayEnabled(6), Eq(expected[6]));
	}
}

TEST(WeeklySchedulerImplFromDtoTest, updateFromWeeklySchedulerDto_empty) {
	for (const auto& sample : Dto2ObjectTest::WeeklySchedulerSampleList()) {
		const WeeklySchedulerImpl& actual = sample.getObject();

		check(WeeklySchedulerDtoUpdateType::Nothing,
				toDays(actual)
			);
	}
}

TEST(WeeklySchedulerImplFromDtoTest, updateFromWeeklySchedulerDto_partial_days) {
	for (const auto& sample : Dto2ObjectTest::WeeklySchedulerSampleList()) {
		const WeeklySchedulerImpl& actual = sample.getObject();

		check(WeeklySchedulerDtoUpdateType::Days,
				toDays(actual)
			);
	}
}

TEST(WeeklySchedulerImplFromDtoTest, updateFromWeeklySchedulerDto_all) {
	for (const auto& sample : Dto2ObjectTest::WeeklySchedulerSampleList()) {
		const WeeklySchedulerImpl& actual = sample.getObject();

		check(WeeklySchedulerDtoUpdateType::All,
				toDays(actual)
			);
	}
}

TEST(WeeklySchedulerImplFromDtoTest, updateFromWeeklySchedulerDto_validSize) {

	WeeklySchedulerImpl actualRunTimeContainer;

	EXPECT_NO_THROW(
			actualRunTimeContainer.updateFromWeeklySchedulerDto(std::list<bool>(7))
		);
}

TEST(WeeklySchedulerImplFromDtoTest, updateFromWeeklySchedulerDto_invalidSize) {

	for (size_t i = 0; i < 10; i++) {

		if (7 == i) {
			continue;
		}

		WeeklySchedulerImpl actualRunTimeContainer;

		EXPECT_THROW(
				actualRunTimeContainer.updateFromWeeklySchedulerDto(std::list<bool>(i)),
				IllegalArgumentException
			);
	}
}
