#include "StartTimeImplTest.h"
#include "Dto2ObjectSamples/StartTimeSamples.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeImplToDtoTest, toStartTimeDto) {
	const Dto2ObjectTestSamples::StartTimeSampleList sampleList;

	ASSERT_THAT(sampleList, SizeIs(4));

	for (const auto& sample : sampleList) {
		const StartTimeImpl& actual = sample.getObject();
		const StartTimeDTO& expected = sample.getDto();

		EXPECT_THAT(actual.toStartTimeDto(), Eq(expected));
	}
}

///////////////////////////////////////////////////////////////////////////////

void StartTimeImplFromDtoTest::checkUpdateFromStartTimeDto(const UpdateType updateType) {
	const auto sampleList = Dto2ObjectTestSamples::StartTimeSampleList();

	unsigned expectedHours = startTime->getHours();
	unsigned expectedMinutes = startTime->getMinutes();

	for (const auto& sample : sampleList) {
		StartTimeDTO actualStartTimeDTO;

		if (UpdateType::Hours == updateType) {
			expectedHours = sample.getDto().getHours();
			expectedMinutes = 0;
			actualStartTimeDTO.setHours(expectedHours);
		}

		if (UpdateType::Minutes == updateType) {
			expectedHours = 0;
			expectedMinutes = sample.getDto().getMinutes();
			actualStartTimeDTO.setMinutes(expectedMinutes);
		}

		if (UpdateType::All == updateType) {
			expectedHours = sample.getDto().getHours();
			expectedMinutes = sample.getDto().getMinutes();
			actualStartTimeDTO.setHours(expectedHours);
			actualStartTimeDTO.setMinutes(expectedMinutes);
		}

		startTime->updateFromStartTimeDto(actualStartTimeDTO);

		EXPECT_THAT(startTime->getHours(), Eq(expectedHours));
		EXPECT_THAT(startTime->getMinutes(), Eq(expectedMinutes));
	}
}

TEST_F(StartTimeImplFromDtoTest, updateFromStartTimeDto_empty) {
	checkUpdateFromStartTimeDto(UpdateType::Nothing);
}

TEST_F(StartTimeImplFromDtoTest, updateFromStartTimeDto_hours) {
	checkUpdateFromStartTimeDto(UpdateType::Hours);
}

TEST_F(StartTimeImplFromDtoTest, updateFromStartTimeDto_minutes) {
	checkUpdateFromStartTimeDto(UpdateType::Minutes);
}

TEST_F(StartTimeImplFromDtoTest, updateFromStartTimeDto_all) {
	checkUpdateFromStartTimeDto(UpdateType::All);
}
