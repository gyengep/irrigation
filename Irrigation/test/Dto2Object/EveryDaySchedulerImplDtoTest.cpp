#include "Schedulers/EveryDaySchedulerImpl.h"
#include "Dto2Object/EveryDaySchedulerSamples.h"
#include <gmock/gmock.h>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

TEST(EveryDaySchedulerImplToDtoTest, toEveryDaySchedulerDto) {
	const Dto2ObjectTest::EveryDaySchedulerSample sample;
	const EveryDaySchedulerImpl& actual = sample.getObject();
	const EveryDaySchedulerDTO& expected = sample.getDto();

	EXPECT_THAT(actual.toEveryDaySchedulerDto(), Eq(expected));
}

TEST(EveryDaySchedulerImplFromDtoTest, updateFromEveryDaySchedulerDto) {
	EveryDaySchedulerImpl actualEveryDayScheduler;
	const EveryDaySchedulerDTO actualEveryDaySchedulerDTO = Dto2ObjectTest::EveryDaySchedulerSample().getDto();
	actualEveryDayScheduler.updateFromEveryDaySchedulerDto(actualEveryDaySchedulerDTO);
}
