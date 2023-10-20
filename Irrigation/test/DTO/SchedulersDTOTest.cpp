#include <gmock/gmock.h>
#include <list>
#include <stdexcept>
#include "DTO/SchedulersDto.h"
#include "DtoTestMacros.h"

using namespace std;
using namespace testing;


TEST(SchedulersDtoTest, defaultConstructor) {
	SchedulersDto schedulersDto;

	EXPECT_FALSE(schedulersDto.hasEveryDayScheduler());
	EXPECT_FALSE(schedulersDto.hasHotWeatherScheduler());
	EXPECT_FALSE(schedulersDto.hasTemperatureDependentScheduler());
	EXPECT_FALSE(schedulersDto.hasWeeklyScheduler());

	EXPECT_THROW(schedulersDto.getEveryDayScheduler(), logic_error);
	EXPECT_THROW(schedulersDto.getHotWeatherScheduler(), logic_error);
	EXPECT_THROW(schedulersDto.getTemperatureDependentScheduler(), logic_error);
	EXPECT_THROW(schedulersDto.getWeeklyScheduler(), logic_error);
}

TEST(SchedulersDtoTest, parametrizedConstructor) {
	const EveryDaySchedulerDto expectedEveryDaySchedulerDto;
	const HotWeatherSchedulerDto expectedHotWeatherSchedulerDto(120, 35.0f);
	const TemperatureDependentSchedulerDto expectedTemperatureDependentSchedulerDto(1.1f, 80, 90);
	const WeeklySchedulerDto expectedWeeklySchedulerDto(list<bool>({ true, false, true }));

	SchedulersDto schedulersDto(
			std::move(EveryDaySchedulerDto(expectedEveryDaySchedulerDto)),
			std::move(HotWeatherSchedulerDto(expectedHotWeatherSchedulerDto)),
			std::move(TemperatureDependentSchedulerDto(expectedTemperatureDependentSchedulerDto)),
			std::move(WeeklySchedulerDto(expectedWeeklySchedulerDto))
		);

	EXPECT_TRUE(schedulersDto.hasEveryDayScheduler());
	EXPECT_TRUE(schedulersDto.hasHotWeatherScheduler());
	EXPECT_TRUE(schedulersDto.hasTemperatureDependentScheduler());
	EXPECT_TRUE(schedulersDto.hasWeeklyScheduler());

	EXPECT_THAT(schedulersDto.getEveryDayScheduler(), Eq(expectedEveryDaySchedulerDto));
	EXPECT_THAT(schedulersDto.getHotWeatherScheduler(), Eq(expectedHotWeatherSchedulerDto));
	EXPECT_THAT(schedulersDto.getTemperatureDependentScheduler(), Eq(expectedTemperatureDependentSchedulerDto));
	EXPECT_THAT(schedulersDto.getWeeklyScheduler(), Eq(expectedWeeklySchedulerDto));
}

TEST(SchedulersDtoTest, copyConstructor) {
	const EveryDaySchedulerDto expectedEveryDaySchedulerDto;
	const HotWeatherSchedulerDto expectedHotWeatherSchedulerDto(90, 31.0f);
	const TemperatureDependentSchedulerDto expectedTemperatureDependentSchedulerDto(2.1f, 180, 190);
	const WeeklySchedulerDto expectedWeeklySchedulerDto(list<bool>({ true, false, true }));

	const SchedulersDto source(
			std::move(EveryDaySchedulerDto(expectedEveryDaySchedulerDto)),
			std::move(HotWeatherSchedulerDto(expectedHotWeatherSchedulerDto)),
			std::move(TemperatureDependentSchedulerDto(expectedTemperatureDependentSchedulerDto)),
			std::move(WeeklySchedulerDto(expectedWeeklySchedulerDto))
		);

	const SchedulersDto schedulersDto(source);

	EXPECT_TRUE(schedulersDto.hasEveryDayScheduler());
	EXPECT_TRUE(schedulersDto.hasHotWeatherScheduler());
	EXPECT_TRUE(schedulersDto.hasTemperatureDependentScheduler());
	EXPECT_TRUE(schedulersDto.hasWeeklyScheduler());

	EXPECT_THAT(schedulersDto.getEveryDayScheduler(), Eq(expectedEveryDaySchedulerDto));
	EXPECT_THAT(schedulersDto.getHotWeatherScheduler(), Eq(expectedHotWeatherSchedulerDto));
	EXPECT_THAT(schedulersDto.getTemperatureDependentScheduler(), Eq(expectedTemperatureDependentSchedulerDto));
	EXPECT_THAT(schedulersDto.getWeeklyScheduler(), Eq(expectedWeeklySchedulerDto));
}

TEST(SchedulersDtoTest, moveConstructor) {
	const EveryDaySchedulerDto expectedEveryDaySchedulerDto;
	const HotWeatherSchedulerDto expectedHotWeatherSchedulerDto(43, 15.0f);
	const TemperatureDependentSchedulerDto expectedTemperatureDependentSchedulerDto(1.1f, 63, 64);
	const WeeklySchedulerDto expectedWeeklySchedulerDto(list<bool>({ true, false, true }));

	SchedulersDto source(
			std::move(EveryDaySchedulerDto(expectedEveryDaySchedulerDto)),
			std::move(HotWeatherSchedulerDto(expectedHotWeatherSchedulerDto)),
			std::move(TemperatureDependentSchedulerDto(expectedTemperatureDependentSchedulerDto)),
			std::move(WeeklySchedulerDto(expectedWeeklySchedulerDto))
		);

	const SchedulersDto schedulersDto(std::move(source));

	EXPECT_TRUE(schedulersDto.hasEveryDayScheduler());
	EXPECT_TRUE(schedulersDto.hasHotWeatherScheduler());
	EXPECT_TRUE(schedulersDto.hasTemperatureDependentScheduler());
	EXPECT_TRUE(schedulersDto.hasWeeklyScheduler());

	EXPECT_THAT(schedulersDto.getEveryDayScheduler(), Eq(expectedEveryDaySchedulerDto));
	EXPECT_THAT(schedulersDto.getHotWeatherScheduler(), Eq(expectedHotWeatherSchedulerDto));
	EXPECT_THAT(schedulersDto.getTemperatureDependentScheduler(), Eq(expectedTemperatureDependentSchedulerDto));
	EXPECT_THAT(schedulersDto.getWeeklyScheduler(), Eq(expectedWeeklySchedulerDto));
}

///////////////////////////////////////////////////////////////////////////////

CHECK_DTO_FUNCTIONS(SchedulersDto, EveryDaySchedulerDto, EveryDayScheduler, EveryDaySchedulerDto());
CHECK_DTO_FUNCTIONS(SchedulersDto, HotWeatherSchedulerDto, HotWeatherScheduler, HotWeatherSchedulerDto(120, 30));
CHECK_DTO_FUNCTIONS(SchedulersDto, TemperatureDependentSchedulerDto, TemperatureDependentScheduler, TemperatureDependentSchedulerDto(0.9f, 75, 110));
CHECK_DTO_FUNCTIONS(SchedulersDto, WeeklySchedulerDto, WeeklyScheduler, WeeklySchedulerDto(list<bool>({ true, false, true, false })));

///////////////////////////////////////////////////////////////////////////////

TEST(SchedulersDtoTest, equalsOperator) {
	SchedulersDto dto1;
	SchedulersDto dto2;

	EXPECT_TRUE(dto1 == dto2);
	EXPECT_TRUE(dto2 == dto1);

	CHECK_DTO_EQUALS_MOVE(HotWeatherSchedulerDto, HotWeatherScheduler,
			HotWeatherSchedulerDto(100, 25.0f),
			HotWeatherSchedulerDto(110, 35.0f)
		);
	CHECK_DTO_EQUALS_MOVE(TemperatureDependentSchedulerDto, TemperatureDependentScheduler,
			TemperatureDependentSchedulerDto(10.0f, 15, 25),
			TemperatureDependentSchedulerDto(15.0f, 15, 25)
		);
	CHECK_DTO_EQUALS_MOVE(WeeklySchedulerDto, WeeklyScheduler,
			WeeklySchedulerDto(list<bool>({ true, true, false })),
			WeeklySchedulerDto(list<bool>({ true, false, false }))
		);
}
