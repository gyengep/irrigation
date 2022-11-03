#include <gmock/gmock.h>
#include <list>
#include <stdexcept>
#include "DTO/SchedulersDTO.h"
#include "DtoTestMacros.h"

using namespace std;
using namespace testing;


TEST(SchedulersDTOTest, defaultConstructor) {
	SchedulersDTO schedulersDTO;

	EXPECT_FALSE(schedulersDTO.hasEveryDayScheduler());
	EXPECT_FALSE(schedulersDTO.hasHotWeatherScheduler());
	EXPECT_FALSE(schedulersDTO.hasTemperatureDependentScheduler());
	EXPECT_FALSE(schedulersDTO.hasWeeklyScheduler());

	EXPECT_THROW(schedulersDTO.getEveryDayScheduler(), logic_error);
	EXPECT_THROW(schedulersDTO.getHotWeatherScheduler(), logic_error);
	EXPECT_THROW(schedulersDTO.getTemperatureDependentScheduler(), logic_error);
	EXPECT_THROW(schedulersDTO.getWeeklyScheduler(), logic_error);
}

TEST(SchedulersDTOTest, parametrizedConstructor) {
	const EveryDaySchedulerDTO expectedEveryDayScheduler;
	const HotWeatherSchedulerDTO expectedHotWeatherScheduler(120, 35.0f);
	const TemperatureDependentSchedulerDTO expectedTemperatureDependentScheduler(1.1f, 80, 90, 120);
	const WeeklySchedulerDTO expectedWeeklyScheduler(list<bool>({ true, false, true }));

	SchedulersDTO schedulersDTO(
			std::move(EveryDaySchedulerDTO(expectedEveryDayScheduler)),
			std::move(HotWeatherSchedulerDTO(expectedHotWeatherScheduler)),
			std::move(TemperatureDependentSchedulerDTO(expectedTemperatureDependentScheduler)),
			std::move(WeeklySchedulerDTO(expectedWeeklyScheduler))
		);

	EXPECT_TRUE(schedulersDTO.hasEveryDayScheduler());
	EXPECT_TRUE(schedulersDTO.hasHotWeatherScheduler());
	EXPECT_TRUE(schedulersDTO.hasTemperatureDependentScheduler());
	EXPECT_TRUE(schedulersDTO.hasWeeklyScheduler());

	EXPECT_THAT(schedulersDTO.getEveryDayScheduler(), Eq(expectedEveryDayScheduler));
	EXPECT_THAT(schedulersDTO.getHotWeatherScheduler(), Eq(expectedHotWeatherScheduler));
	EXPECT_THAT(schedulersDTO.getTemperatureDependentScheduler(), Eq(expectedTemperatureDependentScheduler));
	EXPECT_THAT(schedulersDTO.getWeeklyScheduler(), Eq(expectedWeeklyScheduler));
}

TEST(SchedulersDTOTest, copyConstructor) {
	const EveryDaySchedulerDTO expectedEveryDayScheduler;
	const HotWeatherSchedulerDTO expectedHotWeatherScheduler(90, 31.0f);
	const TemperatureDependentSchedulerDTO expectedTemperatureDependentScheduler(2.1f, 180, 190, 20);
	const WeeklySchedulerDTO expectedWeeklyScheduler(list<bool>({ true, false, true }));

	const SchedulersDTO source(
			std::move(EveryDaySchedulerDTO(expectedEveryDayScheduler)),
			std::move(HotWeatherSchedulerDTO(expectedHotWeatherScheduler)),
			std::move(TemperatureDependentSchedulerDTO(expectedTemperatureDependentScheduler)),
			std::move(WeeklySchedulerDTO(expectedWeeklyScheduler))
		);

	const SchedulersDTO schedulersDTO(source);

	EXPECT_TRUE(schedulersDTO.hasEveryDayScheduler());
	EXPECT_TRUE(schedulersDTO.hasHotWeatherScheduler());
	EXPECT_TRUE(schedulersDTO.hasTemperatureDependentScheduler());
	EXPECT_TRUE(schedulersDTO.hasWeeklyScheduler());

	EXPECT_THAT(schedulersDTO.getEveryDayScheduler(), Eq(expectedEveryDayScheduler));
	EXPECT_THAT(schedulersDTO.getHotWeatherScheduler(), Eq(expectedHotWeatherScheduler));
	EXPECT_THAT(schedulersDTO.getTemperatureDependentScheduler(), Eq(expectedTemperatureDependentScheduler));
	EXPECT_THAT(schedulersDTO.getWeeklyScheduler(), Eq(expectedWeeklyScheduler));
}

TEST(SchedulersDTOTest, moveConstructor) {
	const EveryDaySchedulerDTO expectedEveryDayScheduler;
	const HotWeatherSchedulerDTO expectedHotWeatherScheduler(43, 15.0f);
	const TemperatureDependentSchedulerDTO expectedTemperatureDependentScheduler(1.1f, 63, 64, 65);
	const WeeklySchedulerDTO expectedWeeklyScheduler(list<bool>({ true, false, true }));

	SchedulersDTO source(
			std::move(EveryDaySchedulerDTO(expectedEveryDayScheduler)),
			std::move(HotWeatherSchedulerDTO(expectedHotWeatherScheduler)),
			std::move(TemperatureDependentSchedulerDTO(expectedTemperatureDependentScheduler)),
			std::move(WeeklySchedulerDTO(expectedWeeklyScheduler))
		);

	const SchedulersDTO schedulersDTO(std::move(source));

	EXPECT_TRUE(schedulersDTO.hasEveryDayScheduler());
	EXPECT_TRUE(schedulersDTO.hasHotWeatherScheduler());
	EXPECT_TRUE(schedulersDTO.hasTemperatureDependentScheduler());
	EXPECT_TRUE(schedulersDTO.hasWeeklyScheduler());

	EXPECT_THAT(schedulersDTO.getEveryDayScheduler(), Eq(expectedEveryDayScheduler));
	EXPECT_THAT(schedulersDTO.getHotWeatherScheduler(), Eq(expectedHotWeatherScheduler));
	EXPECT_THAT(schedulersDTO.getTemperatureDependentScheduler(), Eq(expectedTemperatureDependentScheduler));
	EXPECT_THAT(schedulersDTO.getWeeklyScheduler(), Eq(expectedWeeklyScheduler));
}

///////////////////////////////////////////////////////////////////////////////

CHECK_DTO_FUNCTIONS(SchedulersDTO, EveryDaySchedulerDTO, EveryDayScheduler, EveryDaySchedulerDTO());
CHECK_DTO_FUNCTIONS(SchedulersDTO, HotWeatherSchedulerDTO, HotWeatherScheduler, HotWeatherSchedulerDTO(120, 30));
CHECK_DTO_FUNCTIONS(SchedulersDTO, TemperatureDependentSchedulerDTO, TemperatureDependentScheduler, TemperatureDependentSchedulerDTO(0.9f, 75, 110, 90));
CHECK_DTO_FUNCTIONS(SchedulersDTO, WeeklySchedulerDTO, WeeklyScheduler, WeeklySchedulerDTO(list<bool>({ true, false, true, false })));

///////////////////////////////////////////////////////////////////////////////

TEST(SchedulersDTOTest, equalsOperator) {
	SchedulersDTO dto1;
	SchedulersDTO dto2;

	EXPECT_TRUE(dto1 == dto2);
	EXPECT_TRUE(dto2 == dto1);

	CHECK_DTO_EQUALS_MOVE(HotWeatherSchedulerDTO, HotWeatherScheduler,
			HotWeatherSchedulerDTO(100, 25.0f),
			HotWeatherSchedulerDTO(110, 35.0f)
		);
	CHECK_DTO_EQUALS_MOVE(TemperatureDependentSchedulerDTO, TemperatureDependentScheduler,
			TemperatureDependentSchedulerDTO(10.0f, 15, 25, 35),
			TemperatureDependentSchedulerDTO(15.0f, 15, 25, 35)
		);
	CHECK_DTO_EQUALS_MOVE(WeeklySchedulerDTO, WeeklyScheduler,
			WeeklySchedulerDTO(list<bool>({ true, true, false })),
			WeeklySchedulerDTO(list<bool>({ true, false, false }))
		);
}
