#include "StartTimeImplTest.h"
#include "Dto2ObjectSamples/StartTimeSamples.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(StartTimeImplToDtoTest, toStartTimeDto) {
	const Dto2ObjectTestSamples::StartTimeSampleList sampleList;

	ASSERT_THAT(sampleList, SizeIs(4));

	for (const auto& sample : sampleList) {
		const StartTimeImpl& actual = sample.getObject();
		const StartTimeDto& expected = sample.getDto();

		EXPECT_THAT(actual.toStartTimeDto(), Eq(expected));
	}
}

///////////////////////////////////////////////////////////////////////////////

void StartTimeImplFromDtoTest::checkUpdateFromStartTimeDto(const UpdateType updateType) {
	const auto sampleList = Dto2ObjectTestSamples::StartTimeSampleList();

	unsigned expectedHours = startTime->getHours();
	unsigned expectedMinutes = startTime->getMinutes();

	for (const auto& sample : sampleList) {
		StartTimeDto actualStartTimeDto;

		if (UpdateType::Hours == updateType) {
			expectedHours = sample.getDto().getHours();
			expectedMinutes = 0;
			actualStartTimeDto.setHours(expectedHours);
		}

		if (UpdateType::Minutes == updateType) {
			expectedHours = 0;
			expectedMinutes = sample.getDto().getMinutes();
			actualStartTimeDto.setMinutes(expectedMinutes);
		}

		if (UpdateType::All == updateType) {
			expectedHours = sample.getDto().getHours();
			expectedMinutes = sample.getDto().getMinutes();
			actualStartTimeDto.setHours(expectedHours);
			actualStartTimeDto.setMinutes(expectedMinutes);
		}

		startTime->updateFromStartTimeDto(actualStartTimeDto);

		EXPECT_THAT(startTime->getHours(), Eq(expectedHours));
		EXPECT_THAT(startTime->getMinutes(), Eq(expectedMinutes));
	}
}

TEST_F(StartTimeImplFromDtoTest, updateFromStartTimeDto_empty) {
	startTime->set(15, 25);
	checkUpdateFromStartTimeDto(UpdateType::Nothing);
}

TEST_F(StartTimeImplFromDtoTest, updateFromStartTimeDto_hours) {
	startTime->set(16, 26);
	checkUpdateFromStartTimeDto(UpdateType::Hours);
}

TEST_F(StartTimeImplFromDtoTest, updateFromStartTimeDto_minutes) {
	startTime->set(17, 27);
	checkUpdateFromStartTimeDto(UpdateType::Minutes);
}

TEST_F(StartTimeImplFromDtoTest, updateFromStartTimeDto_all) {
	startTime->set(18, 28);
	checkUpdateFromStartTimeDto(UpdateType::All);
}
