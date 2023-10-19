#include "RunTimeImplTest.h"
#include "Dto2ObjectSamples/RunTimeSamples.h"

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

void RunTimeImplFromDtoTest::checkUpdateFromRunTimeDto(const UpdateType updateType) {
	const auto sampleList = Dto2ObjectTestSamples::RunTimeSampleList();

	const std::chrono::seconds seconds = runTime->get();
	unsigned expectedMinutes = seconds.count() / 60;
	unsigned expectedSeconds = seconds.count() % 60;

	for (const auto& sample : sampleList) {
		RunTimeDTO actualRunTimeDTO;

		if (UpdateType::Minutes == updateType) {
			expectedMinutes = sample.getDto().getMinutes();
			expectedSeconds = 0;
			actualRunTimeDTO.setMinutes(expectedMinutes);
		}

		if (UpdateType::Seconds == updateType) {
			expectedMinutes = 0;
			expectedSeconds = sample.getDto().getSeconds();
			actualRunTimeDTO.setSeconds(expectedSeconds);
		}

		if (UpdateType::All == updateType) {
			expectedMinutes = sample.getDto().getMinutes();
			expectedSeconds = sample.getDto().getSeconds();
			actualRunTimeDTO.setMinutes(expectedMinutes);
			actualRunTimeDTO.setSeconds(expectedSeconds);
		}

		runTime->updateFromRunTimeDto(actualRunTimeDTO);

		EXPECT_THAT(runTime->get(), Eq(std::chrono::minutes(expectedMinutes) + std::chrono::seconds(expectedSeconds)));
	}
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RunTimeImplFromDtoTest, updateFromRunTimeDto_empty) {
	checkUpdateFromRunTimeDto(UpdateType::Nothing);
}

TEST_F(RunTimeImplFromDtoTest, updateFromRunTimeDto_minutes) {
	checkUpdateFromRunTimeDto(UpdateType::Minutes);
}

TEST_F(RunTimeImplFromDtoTest, updateFromRunTimeDto_seconds) {
	checkUpdateFromRunTimeDto(UpdateType::Seconds);
}

TEST_F(RunTimeImplFromDtoTest, updateFromRunTimeDto_all) {
	checkUpdateFromRunTimeDto(UpdateType::All);
}
