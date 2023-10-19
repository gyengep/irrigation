#include "Schedulers/Impl/EveryDaySchedulerImpl.h"
#include "Dto2ObjectSamples/EveryDaySchedulerSamples.h"
#include <gmock/gmock.h>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(EveryDaySchedulerImplToDtoTest, toEveryDaySchedulerDto) {
	const Dto2ObjectTestSamples::EveryDaySchedulerSample sample;
	const EveryDaySchedulerImpl& actual = sample.getObject();
	const EveryDaySchedulerDTO& expected = sample.getDto();

	EXPECT_THAT(actual.toEveryDaySchedulerDto(), Eq(expected));
}

TEST(EveryDaySchedulerImplFromDtoTest, updateFromEveryDaySchedulerDto) {
	EveryDaySchedulerImpl actualEveryDayScheduler;
	const EveryDaySchedulerDTO actualEveryDaySchedulerDTO = Dto2ObjectTestSamples::EveryDaySchedulerSample().getDto();
	actualEveryDayScheduler.updateFromEveryDaySchedulerDto(actualEveryDaySchedulerDTO);
}
