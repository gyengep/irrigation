#include "HotWeatherSchedulerImplTest.h"
#include "Dto2ObjectSamples/HotWeatherSchedulerSamples.h"

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(HotWeatherSchedulerImplToDtoTest, toHotWeatherSchedulerDto) {
	const Dto2ObjectTestSamples::HotWeatherSchedulerSampleList sampleList;

	ASSERT_THAT(sampleList, SizeIs(4));

	for (const auto& sample : sampleList) {
		const HotWeatherSchedulerImpl& actual = sample.getObject();
		const HotWeatherSchedulerDto& expected = sample.getDto();

		EXPECT_THAT(actual.toHotWeatherSchedulerDto(), Eq(expected));
	}
}

///////////////////////////////////////////////////////////////////////////////

void HotWeatherSchedulerImplFromDtoTest::checkUpdateFromHotWeatherSchedulerDto(const UpdateType updateType) {
	const auto sampleList = Dto2ObjectTestSamples::HotWeatherSchedulerSampleList();

	std::chrono::seconds expectedPeriod = hotWeatherScheduler->getPeriod();
	float expectedMinTemperature = hotWeatherScheduler->getMinTemperature();

	for (const auto& sample : sampleList) {
		HotWeatherSchedulerDto actualHotWeatherSchedulerDto;

		if (UpdateType::Period == updateType || UpdateType::All == updateType) {
			expectedPeriod = std::chrono::seconds(sample.getDto().getPeriodInSeconds());
			actualHotWeatherSchedulerDto.setPeriodInSeconds(std::chrono::duration_cast<std::chrono::seconds>(expectedPeriod).count());
		}

		if (UpdateType::MinTemperature == updateType || UpdateType::All == updateType) {
			expectedMinTemperature = sample.getDto().getMinTemperature();
			actualHotWeatherSchedulerDto.setMinTemperature(expectedMinTemperature);
		}

		hotWeatherScheduler->updateFromHotWeatherSchedulerDto(actualHotWeatherSchedulerDto);

		EXPECT_THAT(hotWeatherScheduler->getPeriod(), Eq(expectedPeriod));
		EXPECT_THAT(hotWeatherScheduler->getMinTemperature(), Eq(expectedMinTemperature));
	}
}

TEST_F(HotWeatherSchedulerImplFromDtoTest, updateFromHotWeatherSchedulerDto_empty) {
	hotWeatherScheduler->setMinTemperature(83);
	hotWeatherScheduler->setPeriod(std::chrono::seconds(33));

	checkUpdateFromHotWeatherSchedulerDto(UpdateType::Nothing);
}

TEST_F(HotWeatherSchedulerImplFromDtoTest, updateFromHotWeatherSchedulerDto_partial_minTemperature) {
	hotWeatherScheduler->setMinTemperature(84);
	hotWeatherScheduler->setPeriod(std::chrono::seconds(34));

	checkUpdateFromHotWeatherSchedulerDto(UpdateType::MinTemperature);
}

TEST_F(HotWeatherSchedulerImplFromDtoTest, updateFromHotWeatherSchedulerDto_partial_period) {
	hotWeatherScheduler->setMinTemperature(85);
	hotWeatherScheduler->setPeriod(std::chrono::seconds(35));

	checkUpdateFromHotWeatherSchedulerDto(UpdateType::Period);
}

TEST_F(HotWeatherSchedulerImplFromDtoTest, updateFromHotWeatherSchedulerDto_all) {
	hotWeatherScheduler->setMinTemperature(56);
	hotWeatherScheduler->setPeriod(std::chrono::seconds(36));

	checkUpdateFromHotWeatherSchedulerDto(UpdateType::All);
}
