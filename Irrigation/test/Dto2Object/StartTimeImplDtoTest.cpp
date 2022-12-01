#include "Logic/StartTimeImpl.h"
#include "Dto2Object/StartTimeSamples.h"
#include <gmock/gmock.h>
#include <memory>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeImplToDtoTest, toStartTimeDto) {
	const Dto2ObjectTest::StartTimeSampleList sampleList;

	ASSERT_THAT(sampleList, SizeIs(4));

	for (const auto& sample : sampleList) {
		const StartTimeImpl& actual = sample.getObject();
		const StartTimeDTO& expected = sample.getDto();

		EXPECT_THAT(actual.toStartTimeDto(), Eq(expected));
	}
}

///////////////////////////////////////////////////////////////////////////////

enum class StartTimeDtoUpdateType {
	Nothing,
	Hours,
	Minutes,
	All
};

void check(const StartTimeDtoUpdateType updateType, unsigned hours, unsigned minutes) {

	StartTimeImpl actualStartTime(
		hours,
		minutes
	);

	unsigned expectedHours = hours;
	unsigned expectedMinutes = minutes;

	for (const auto& sample : Dto2ObjectTest::StartTimeSampleList()) {
		StartTimeDTO actualStartTimeDTO;

		if (StartTimeDtoUpdateType::Hours == updateType) {
			expectedHours = sample.getDto().getHours();
			expectedMinutes = 0;
			actualStartTimeDTO.setHours(expectedHours);
		}

		if (StartTimeDtoUpdateType::Minutes == updateType) {
			expectedHours = 0;
			expectedMinutes = sample.getDto().getMinutes();
			actualStartTimeDTO.setMinutes(expectedMinutes);
		}

		if (StartTimeDtoUpdateType::All == updateType) {
			expectedHours = sample.getDto().getHours();
			expectedMinutes = sample.getDto().getMinutes();
			actualStartTimeDTO.setHours(expectedHours);
			actualStartTimeDTO.setMinutes(expectedMinutes);
		}

		actualStartTime.updateFromStartTimeDto(actualStartTimeDTO);

		EXPECT_THAT(actualStartTime.getHours(), Eq(expectedHours));
		EXPECT_THAT(actualStartTime.getMinutes(), Eq(expectedMinutes));
	}
}

TEST(StartTimeImplFromDtoTest, updateFromStartTimeDto_empty) {
	for (const auto& sample : Dto2ObjectTest::StartTimeSampleList()) {
		const StartTimeImpl& actual = sample.getObject();

		check(StartTimeDtoUpdateType::Nothing,
				actual.getHours(),
				actual.getMinutes()
			);
	}
}

TEST(StartTimeImplFromDtoTest, updateFromStartTimeDto_hours) {
	for (const auto& sample : Dto2ObjectTest::StartTimeSampleList()) {
		const StartTimeImpl& actual = sample.getObject();

		check(StartTimeDtoUpdateType::Hours,
				actual.getHours(),
				actual.getMinutes()
			);
	}
}

TEST(StartTimeImplFromDtoTest, updateFromStartTimeDto_minutes) {
	for (const auto& sample : Dto2ObjectTest::StartTimeSampleList()) {
		const StartTimeImpl& actual = sample.getObject();

		check(StartTimeDtoUpdateType::Minutes,
				actual.getHours(),
				actual.getMinutes()
			);
	}
}

TEST(StartTimeImplFromDtoTest, updateFromStartTimeDto_all) {
	for (const auto& sample : Dto2ObjectTest::StartTimeSampleList()) {
		const StartTimeImpl& actual = sample.getObject();

		check(StartTimeDtoUpdateType::All,
				actual.getHours(),
				actual.getMinutes()
			);
	}
}
