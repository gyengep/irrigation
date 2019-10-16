#include <gmock/gmock.h>
#include <list>
#include <stdexcept>
#include "DTO/ProgramDTO.h"
#include "DtoTestMacros.h"

using namespace std;
using namespace testing;


TEST(ProgramDTOTest, defaultConstructor) {
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasId());
	EXPECT_FALSE(programDTO.hasDisabled());
	EXPECT_FALSE(programDTO.hasName());
	EXPECT_FALSE(programDTO.hasAdjustment());
	EXPECT_FALSE(programDTO.hasSchedulerType());
	EXPECT_FALSE(programDTO.hasEveryDayScheduler());
	EXPECT_FALSE(programDTO.hasHotWeatherScheduler());
	EXPECT_FALSE(programDTO.hasPeriodicScheduler());
	EXPECT_FALSE(programDTO.hasTemperatureDependentScheduler());
	EXPECT_FALSE(programDTO.hasWeeklyScheduler());
	EXPECT_FALSE(programDTO.hasRunTimes());
	EXPECT_FALSE(programDTO.hasStartTimes());

	EXPECT_THROW(programDTO.getId(), logic_error);
	EXPECT_THROW(programDTO.getDisabled(), logic_error);
	EXPECT_THROW(programDTO.getName(), logic_error);
	EXPECT_THROW(programDTO.getAdjustment(), logic_error);
	EXPECT_THROW(programDTO.getSchedulerType(), logic_error);
	EXPECT_THROW(programDTO.getEveryDayScheduler(), logic_error);
	EXPECT_THROW(programDTO.getHotWeatherScheduler(), logic_error);
	EXPECT_THROW(programDTO.getPeriodicScheduler(), logic_error);
	EXPECT_THROW(programDTO.getTemperatureDependentScheduler(), logic_error);
	EXPECT_THROW(programDTO.getWeeklyScheduler(), logic_error);
	EXPECT_THROW(programDTO.getRunTimes(), logic_error);
	EXPECT_THROW(programDTO.getStartTimes(), logic_error);
}

TEST(ProgramDTOTest, parametrizedConstructor) {
	const bool expectedDisabled = true;
	const string expectedName("name");
	const unsigned expectedAdjustment = 78;
	const string expectedSchedulerType("scheduler");
	const EveryDaySchedulerDTO expectedEveryDayScheduler;
	const HotWeatherSchedulerDTO expectedHotWeatherScheduler(120, 35.0f);
	const PeriodicSchedulerDTO expectedPeriodicScheduler(list<bool>({ true, false, true }), 2018, 11, 15);
	const TemperatureDependentSchedulerDTO expectedTemperatureDependentScheduler(1.1f, 80, 90, 120);
	const WeeklySchedulerDTO expectedWeeklyScheduler(list<bool>({ true, false, true }));
	const list<RunTimeDTO> expectedRunTimes({ RunTimeDTO(10, 0), RunTimeDTO(15, 0), RunTimeDTO(20, 0) });
	const list<StartTimeDTO> expectedStartTimes({ StartTimeDTO(5, 0), StartTimeDTO(6, 0) });

	ProgramDTO programDTO(
			expectedDisabled,
			expectedName,
			expectedAdjustment,
			expectedSchedulerType,
			EveryDaySchedulerDTO(expectedEveryDayScheduler),
			HotWeatherSchedulerDTO(expectedHotWeatherScheduler),
			PeriodicSchedulerDTO(expectedPeriodicScheduler),
			TemperatureDependentSchedulerDTO(expectedTemperatureDependentScheduler),
			WeeklySchedulerDTO(expectedWeeklyScheduler),
			list<RunTimeDTO>(expectedRunTimes),
			list<StartTimeDTO>(expectedStartTimes));

	EXPECT_FALSE(programDTO.hasId());
	EXPECT_TRUE(programDTO.hasDisabled());
	EXPECT_TRUE(programDTO.hasName());
	EXPECT_TRUE(programDTO.hasAdjustment());
	EXPECT_TRUE(programDTO.hasSchedulerType());
	EXPECT_TRUE(programDTO.hasEveryDayScheduler());
	EXPECT_TRUE(programDTO.hasHotWeatherScheduler());
	EXPECT_TRUE(programDTO.hasPeriodicScheduler());
	EXPECT_TRUE(programDTO.hasTemperatureDependentScheduler());
	EXPECT_TRUE(programDTO.hasWeeklyScheduler());
	EXPECT_TRUE(programDTO.hasRunTimes());
	EXPECT_TRUE(programDTO.hasStartTimes());

	EXPECT_THAT(programDTO.getDisabled(), Eq(expectedDisabled));
	EXPECT_THAT(programDTO.getName(), Eq(expectedName));
	EXPECT_THAT(programDTO.getAdjustment(), Eq(expectedAdjustment));
	EXPECT_THAT(programDTO.getSchedulerType(), Eq(expectedSchedulerType));
	EXPECT_THAT(programDTO.getEveryDayScheduler(), Eq(expectedEveryDayScheduler));
	EXPECT_THAT(programDTO.getHotWeatherScheduler(), Eq(expectedHotWeatherScheduler));
	EXPECT_THAT(programDTO.getPeriodicScheduler(), Eq(expectedPeriodicScheduler));
	EXPECT_THAT(programDTO.getTemperatureDependentScheduler(), Eq(expectedTemperatureDependentScheduler));
	EXPECT_THAT(programDTO.getWeeklyScheduler(), Eq(expectedWeeklyScheduler));
	EXPECT_THAT(programDTO.getRunTimes(), ContainerEq(expectedRunTimes));
	EXPECT_THAT(programDTO.getStartTimes(), ContainerEq(expectedStartTimes));
}

TEST(ProgramDTOTest, copyConstructor) {
	const bool expectedDisabled = false;
	const string expectedName("name");
	const unsigned expectedAdjustment = 85;
	const string expectedSchedulerType("scheduler");
	const EveryDaySchedulerDTO expectedEveryDayScheduler;
	const HotWeatherSchedulerDTO expectedHotWeatherScheduler(90, 31.0f);
	const PeriodicSchedulerDTO expectedPeriodicScheduler(list<bool>({ true, false, true }), 2018, 11, 15);
	const TemperatureDependentSchedulerDTO expectedTemperatureDependentScheduler(2.1f, 180, 190, 20);
	const WeeklySchedulerDTO expectedWeeklyScheduler(list<bool>({ true, false, true }));
	const list<RunTimeDTO> expectedRunTimes({ RunTimeDTO(0, 90), RunTimeDTO(5, 0) });
	const list<StartTimeDTO> expectedStartTimes({ StartTimeDTO(1, 30), StartTimeDTO(4, 0), StartTimeDTO(6, 0) });

	const ProgramDTO source(
			expectedDisabled,
			expectedName,
			expectedAdjustment,
			expectedSchedulerType,
			EveryDaySchedulerDTO(expectedEveryDayScheduler),
			HotWeatherSchedulerDTO(expectedHotWeatherScheduler),
			PeriodicSchedulerDTO(expectedPeriodicScheduler),
			TemperatureDependentSchedulerDTO(expectedTemperatureDependentScheduler),
			WeeklySchedulerDTO(expectedWeeklyScheduler),
			list<RunTimeDTO>(expectedRunTimes),
			list<StartTimeDTO>(expectedStartTimes));

	const ProgramDTO programDTO(source);

	EXPECT_FALSE(programDTO.hasId());
	EXPECT_TRUE(programDTO.hasDisabled());
	EXPECT_TRUE(programDTO.hasName());
	EXPECT_TRUE(programDTO.hasAdjustment());
	EXPECT_TRUE(programDTO.hasSchedulerType());
	EXPECT_TRUE(programDTO.hasEveryDayScheduler());
	EXPECT_TRUE(programDTO.hasHotWeatherScheduler());
	EXPECT_TRUE(programDTO.hasPeriodicScheduler());
	EXPECT_TRUE(programDTO.hasTemperatureDependentScheduler());
	EXPECT_TRUE(programDTO.hasWeeklyScheduler());
	EXPECT_TRUE(programDTO.hasRunTimes());
	EXPECT_TRUE(programDTO.hasStartTimes());

	EXPECT_THAT(programDTO.getDisabled(), Eq(expectedDisabled));
	EXPECT_THAT(programDTO.getName(), Eq(expectedName));
	EXPECT_THAT(programDTO.getAdjustment(), Eq(expectedAdjustment));
	EXPECT_THAT(programDTO.getSchedulerType(), Eq(expectedSchedulerType));
	EXPECT_THAT(programDTO.getEveryDayScheduler(), Eq(expectedEveryDayScheduler));
	EXPECT_THAT(programDTO.getHotWeatherScheduler(), Eq(expectedHotWeatherScheduler));
	EXPECT_THAT(programDTO.getPeriodicScheduler(), Eq(expectedPeriodicScheduler));
	EXPECT_THAT(programDTO.getTemperatureDependentScheduler(), Eq(expectedTemperatureDependentScheduler));
	EXPECT_THAT(programDTO.getWeeklyScheduler(), Eq(expectedWeeklyScheduler));
	EXPECT_THAT(programDTO.getRunTimes(), ContainerEq(expectedRunTimes));
	EXPECT_THAT(programDTO.getStartTimes(), ContainerEq(expectedStartTimes));
}

TEST(ProgramDTOTest, moveConstructor) {
	const bool expectedDisabled = true;
	const string expectedName("name");
	const unsigned expectedAdjustment = 93;
	const string expectedSchedulerType("scheduler");
	const EveryDaySchedulerDTO expectedEveryDayScheduler;
	const HotWeatherSchedulerDTO expectedHotWeatherScheduler(43, 15.0f);
	const PeriodicSchedulerDTO expectedPeriodicScheduler(list<bool>({ false, true }), 2018, 11, 10);
	const TemperatureDependentSchedulerDTO expectedTemperatureDependentScheduler(1.1f, 63, 64, 65);
	const WeeklySchedulerDTO expectedWeeklyScheduler(list<bool>({ true, false, true }));
	const list<RunTimeDTO> expectedRunTimes({ RunTimeDTO(0, 90), RunTimeDTO(5, 0) });
	const list<StartTimeDTO> expectedStartTimes({ StartTimeDTO(1, 30), StartTimeDTO(4, 0), StartTimeDTO(6, 0) });

	ProgramDTO source(
			expectedDisabled,
			expectedName,
			expectedAdjustment,
			expectedSchedulerType,
			EveryDaySchedulerDTO(expectedEveryDayScheduler),
			HotWeatherSchedulerDTO(expectedHotWeatherScheduler),
			PeriodicSchedulerDTO(expectedPeriodicScheduler),
			TemperatureDependentSchedulerDTO(expectedTemperatureDependentScheduler),
			WeeklySchedulerDTO(expectedWeeklyScheduler),
			list<RunTimeDTO>(expectedRunTimes),
			list<StartTimeDTO>(expectedStartTimes));

	const ProgramDTO programDTO(move(source));

	EXPECT_FALSE(programDTO.hasId());
	EXPECT_TRUE(programDTO.hasDisabled());
	EXPECT_TRUE(programDTO.hasName());
	EXPECT_TRUE(programDTO.hasAdjustment());
	EXPECT_TRUE(programDTO.hasSchedulerType());
	EXPECT_TRUE(programDTO.hasEveryDayScheduler());
	EXPECT_TRUE(programDTO.hasHotWeatherScheduler());
	EXPECT_TRUE(programDTO.hasPeriodicScheduler());
	EXPECT_TRUE(programDTO.hasTemperatureDependentScheduler());
	EXPECT_TRUE(programDTO.hasWeeklyScheduler());
	EXPECT_TRUE(programDTO.hasRunTimes());
	EXPECT_TRUE(programDTO.hasStartTimes());

	EXPECT_THAT(programDTO.getDisabled(), Eq(expectedDisabled));
	EXPECT_THAT(programDTO.getName(), Eq(expectedName));
	EXPECT_THAT(programDTO.getAdjustment(), Eq(expectedAdjustment));
	EXPECT_THAT(programDTO.getSchedulerType(), Eq(expectedSchedulerType));
	EXPECT_THAT(programDTO.getEveryDayScheduler(), Eq(expectedEveryDayScheduler));
	EXPECT_THAT(programDTO.getHotWeatherScheduler(), Eq(expectedHotWeatherScheduler));
	EXPECT_THAT(programDTO.getPeriodicScheduler(), Eq(expectedPeriodicScheduler));
	EXPECT_THAT(programDTO.getTemperatureDependentScheduler(), Eq(expectedTemperatureDependentScheduler));
	EXPECT_THAT(programDTO.getWeeklyScheduler(), Eq(expectedWeeklyScheduler));
	EXPECT_THAT(programDTO.getRunTimes(), ContainerEq(expectedRunTimes));
	EXPECT_THAT(programDTO.getStartTimes(), ContainerEq(expectedStartTimes));
}

///////////////////////////////////////////////////////////////////////////////

CHECK_DTO_FUNCTIONS(ProgramDTO, unsigned, Id, 135);
CHECK_DTO_FUNCTIONS(ProgramDTO, bool, Disabled, true);
CHECK_DTO_FUNCTIONS(ProgramDTO, string, Name, "123456");
CHECK_DTO_FUNCTIONS(ProgramDTO, unsigned, Adjustment, 123);
CHECK_DTO_FUNCTIONS(ProgramDTO, string, SchedulerType, "qwertzuio");
CHECK_DTO_FUNCTIONS(ProgramDTO, EveryDaySchedulerDTO, EveryDayScheduler, EveryDaySchedulerDTO());
CHECK_DTO_FUNCTIONS(ProgramDTO, HotWeatherSchedulerDTO, HotWeatherScheduler, HotWeatherSchedulerDTO(120, 30));
CHECK_DTO_FUNCTIONS(ProgramDTO, PeriodicSchedulerDTO, PeriodicScheduler, PeriodicSchedulerDTO(list<bool>({ true, false }), 1990, 10, 11));
CHECK_DTO_FUNCTIONS(ProgramDTO, TemperatureDependentSchedulerDTO, TemperatureDependentScheduler, TemperatureDependentSchedulerDTO(0.9f, 75, 110, 90));
CHECK_DTO_FUNCTIONS(ProgramDTO, WeeklySchedulerDTO, WeeklyScheduler, WeeklySchedulerDTO(list<bool>({ true, false, true, false })));
CHECK_DTO_FUNCTIONS(ProgramDTO, std::list<RunTimeDTO>, RunTimes, list<RunTimeDTO>({ RunTimeDTO(11, 10), RunTimeDTO(11, 55), RunTimeDTO(12, 0) }));
CHECK_DTO_FUNCTIONS(ProgramDTO, std::list<StartTimeDTO>, StartTimes, list<StartTimeDTO>({ StartTimeDTO(21, 20), StartTimeDTO(22, 40), StartTimeDTO(1, 23) }));

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramDTOTest, equalsOperator) {
	ProgramDTO dto1;
	ProgramDTO dto2;

	EXPECT_TRUE(dto1 == dto2);
	EXPECT_TRUE(dto2 == dto1);

	CHECK_DTO_EQUALS_COPY(unsigned, Id, 5131, 444);
	CHECK_DTO_EQUALS_COPY(bool, Disabled, true, false);
	CHECK_DTO_EQUALS_COPY(string, Name, "abc", "123");
	CHECK_DTO_EQUALS_COPY(unsigned, Adjustment, 98, 63);
	CHECK_DTO_EQUALS_COPY(string, SchedulerType, "987654321", "schedulerType");

	CHECK_DTO_EQUALS_MOVE(HotWeatherSchedulerDTO, HotWeatherScheduler,
			HotWeatherSchedulerDTO(100, 25.0f),
			HotWeatherSchedulerDTO(110, 35.0f)
		);
	CHECK_DTO_EQUALS_MOVE(PeriodicSchedulerDTO, PeriodicScheduler,
			PeriodicSchedulerDTO(list<bool>({ true, false, true }), 2018, 11, 15),
			PeriodicSchedulerDTO(list<bool>({ true, false, false }), 2015, 10, 15)
		);
	CHECK_DTO_EQUALS_MOVE(TemperatureDependentSchedulerDTO, TemperatureDependentScheduler,
			TemperatureDependentSchedulerDTO(10.0f, 15, 25, 35),
			TemperatureDependentSchedulerDTO(15.0f, 15, 25, 35)
		);
	CHECK_DTO_EQUALS_MOVE(WeeklySchedulerDTO, WeeklyScheduler,
			WeeklySchedulerDTO(list<bool>({ true, true, false })),
			WeeklySchedulerDTO(list<bool>({ true, false, false }))
		);
	CHECK_DTO_EQUALS_MOVE(list<RunTimeDTO>, RunTimes,
			list<RunTimeDTO>({ RunTimeDTO(10, 11), RunTimeDTO(20, 21) }),
			list<RunTimeDTO>({ RunTimeDTO(12, 13), RunTimeDTO(22, 23), RunTimeDTO(32, 33) })
		);
	CHECK_DTO_EQUALS_MOVE(list<StartTimeDTO>, StartTimes,
			list<StartTimeDTO>({ StartTimeDTO(17, 23), StartTimeDTO(4, 26), StartTimeDTO(5, 12) }),
			list<StartTimeDTO>({ StartTimeDTO(16, 17), StartTimeDTO(18, 19), StartTimeDTO(20, 21) })
		);
}
