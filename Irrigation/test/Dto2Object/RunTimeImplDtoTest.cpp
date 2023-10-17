#include "Logic/Impl/RunTimeImpl.h"
#include "Samples/RunTimeSamples.h"
#include <gmock/gmock.h>
#include <memory>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeImplToDtoTest, toRunTimeDto) {
	const Dto2ObjectTestSamples::RunTimeSampleList sampleList;

	ASSERT_THAT(sampleList, SizeIs(6));

	for (const auto& sample : sampleList) {
		const RunTimeImpl& actual = sample.getObject();
		const RunTimeDTO& expected = sample.getDto();

		EXPECT_THAT(actual.toRunTimeDto(), Eq(expected));
	}
}

///////////////////////////////////////////////////////////////////////////////

enum class RunTimeDtoUpdateType {
	Nothing,
	Minutes,
	Seconds,
	All
};

void check(const RunTimeDtoUpdateType updateType, unsigned seconds) {

	RunTimeImpl actualRunTime(
		seconds
	);

	unsigned expectedMinutes = seconds / 60;
	unsigned expectedSeconds = seconds % 60;

	for (const auto& sample : Dto2ObjectTestSamples::RunTimeSampleList()) {
		RunTimeDTO actualRunTimeDTO;

		if (RunTimeDtoUpdateType::Minutes == updateType) {
			expectedMinutes = sample.getDto().getSeconds() / 60;
			expectedSeconds = 0;
			actualRunTimeDTO.setMinutes(expectedMinutes);
		}

		if (RunTimeDtoUpdateType::Seconds == updateType) {
			expectedMinutes = 0;
			expectedSeconds = sample.getDto().getSeconds();
			actualRunTimeDTO.setSeconds(expectedSeconds);
		}

		if (RunTimeDtoUpdateType::All == updateType) {
			expectedMinutes = sample.getObject().getSeconds() / 60;
			expectedSeconds = sample.getObject().getSeconds() / 60;
			actualRunTimeDTO.setMinutes(expectedMinutes);
			actualRunTimeDTO.setSeconds(expectedSeconds);
		}

		actualRunTime.updateFromRunTimeDto(actualRunTimeDTO);

		EXPECT_THAT(actualRunTime.getSeconds(), Eq(expectedMinutes * 60 + expectedSeconds));
	}
}

TEST(RunTimeImplFromDtoTest, updateFromRunTimeDto_empty) {
	for (const auto& sample : Dto2ObjectTestSamples::RunTimeSampleList()) {
		const RunTimeImpl& actual = sample.getObject();

		check(RunTimeDtoUpdateType::Nothing,
				actual.getSeconds()
			);
	}
}

TEST(RunTimeImplFromDtoTest, updateFromRunTimeDto_minutes) {
	for (const auto& sample : Dto2ObjectTestSamples::RunTimeSampleList()) {
		const RunTimeImpl& actual = sample.getObject();

		check(RunTimeDtoUpdateType::Minutes,
				actual.getSeconds()
			);
	}
}

TEST(RunTimeImplFromDtoTest, updateFromRunTimeDto_seconds) {
	for (const auto& sample : Dto2ObjectTestSamples::RunTimeSampleList()) {
		const RunTimeImpl& actual = sample.getObject();

		check(RunTimeDtoUpdateType::Minutes,
				actual.getSeconds()
			);
	}
}

TEST(RunTimeImplFromDtoTest, updateFromRunTimeDto_all) {
	for (const auto& sample : Dto2ObjectTestSamples::RunTimeSampleList()) {
		const RunTimeImpl& actual = sample.getObject();

		check(RunTimeDtoUpdateType::All,
				actual.getSeconds()
			);
	}
}
