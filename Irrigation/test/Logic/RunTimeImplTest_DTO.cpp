#include "RunTimeImplTest.h"
#include "Dto2ObjectSamples/RunTimeSamples.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(RunTimeImplToDtoTest, toRunTimeDto) {
	const Dto2ObjectTestSamples::RunTimeSampleList sampleList;

	ASSERT_THAT(sampleList, SizeIs(6));

	for (const auto& sample : sampleList) {
		const RunTimeImpl& actual = sample.getObject();
		const RunTimeDto& expected = sample.getDto();

		EXPECT_THAT(actual.toRunTimeDto(), Eq(expected));
	}
}

///////////////////////////////////////////////////////////////////////////////

void RunTimeImplFromDtoTest::checkUpdateFromRunTimeDto(const UpdateType updateType) {
	const auto sampleList = Dto2ObjectTestSamples::RunTimeSampleList();

	const std::chrono::seconds seconds = runTime->get();
	unsigned expectedMinutes = seconds.count() / 60;
	unsigned expectedSeconds = seconds.count() % 60;

	for (const auto& sample : sampleList) {
		RunTimeDto actualRunTimeDto;

		if (UpdateType::Minutes == updateType) {
			expectedMinutes = sample.getDto().getMinutes();
			expectedSeconds = 0;
			actualRunTimeDto.setMinutes(expectedMinutes);
		}

		if (UpdateType::Seconds == updateType) {
			expectedMinutes = 0;
			expectedSeconds = sample.getDto().getSeconds();
			actualRunTimeDto.setSeconds(expectedSeconds);
		}

		if (UpdateType::All == updateType) {
			expectedMinutes = sample.getDto().getMinutes();
			expectedSeconds = sample.getDto().getSeconds();
			actualRunTimeDto.setMinutes(expectedMinutes);
			actualRunTimeDto.setSeconds(expectedSeconds);
		}

		runTime->updateFromRunTimeDto(actualRunTimeDto);

		EXPECT_THAT(runTime->get(), Eq(std::chrono::minutes(expectedMinutes) + std::chrono::seconds(expectedSeconds)));
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RunTimeImplFromDtoTest, updateFromRunTimeDto_empty) {
	runTime->set(std::chrono::minutes(15) + std::chrono::seconds(25));
	checkUpdateFromRunTimeDto(UpdateType::Nothing);
}

TEST_F(RunTimeImplFromDtoTest, updateFromRunTimeDto_minutes) {
	runTime->set(std::chrono::minutes(16) + std::chrono::seconds(26));
	checkUpdateFromRunTimeDto(UpdateType::Minutes);
}

TEST_F(RunTimeImplFromDtoTest, updateFromRunTimeDto_seconds) {
	runTime->set(std::chrono::minutes(17) + std::chrono::seconds(27));
	checkUpdateFromRunTimeDto(UpdateType::Seconds);
}

TEST_F(RunTimeImplFromDtoTest, updateFromRunTimeDto_all) {
	runTime->set(std::chrono::minutes(18) + std::chrono::seconds(28));
	checkUpdateFromRunTimeDto(UpdateType::All);
}
