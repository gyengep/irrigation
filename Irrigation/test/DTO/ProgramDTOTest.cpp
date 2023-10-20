#include <gmock/gmock.h>
#include <list>
#include <stdexcept>
#include "DTO/ProgramDto.h"
#include "DtoTestMacros.h"

using namespace std;
using namespace testing;


TEST(ProgramDtoTest, defaultConstructor) {
	ProgramDto programDto;

	EXPECT_FALSE(programDto.hasId());
	EXPECT_FALSE(programDto.hasEnabled());
	EXPECT_FALSE(programDto.hasName());
	EXPECT_FALSE(programDto.hasAdjustment());
	EXPECT_FALSE(programDto.hasSchedulerType());
	EXPECT_FALSE(programDto.hasSchedulers());
	EXPECT_FALSE(programDto.hasRunTimes());
	EXPECT_FALSE(programDto.hasStartTimes());

	EXPECT_THROW(programDto.getId(), logic_error);
	EXPECT_THROW(programDto.getEnabled(), logic_error);
	EXPECT_THROW(programDto.getName(), logic_error);
	EXPECT_THROW(programDto.getAdjustment(), logic_error);
	EXPECT_THROW(programDto.getSchedulerType(), logic_error);
	EXPECT_THROW(programDto.getSchedulers(), logic_error);
	EXPECT_THROW(programDto.getRunTimes(), logic_error);
	EXPECT_THROW(programDto.getStartTimes(), logic_error);
}

TEST(ProgramDtoTest, parametrizedConstructor) {
	const bool expectedEnabled = false;
	const string expectedName("name");
	const unsigned expectedAdjustment = 78;
	const string expectedSchedulerType("scheduler");
	const SchedulersDto expectedSchedulers(
			EveryDaySchedulerDto(),
			HotWeatherSchedulerDto(120, 35.0f),
			TemperatureDependentSchedulerDto(1.1f, 80, 90),
			WeeklySchedulerDto(list<bool>({ true, false, true }))
		);
	const list<RunTimeDto> expectedRunTimes({ RunTimeDto(10, 0), RunTimeDto(15, 0), RunTimeDto(20, 0) });
	const list<StartTimeDto> expectedStartTimes({ StartTimeDto(5, 0), StartTimeDto(6, 0) });

	ProgramDto programDto(
			expectedEnabled,
			expectedName,
			expectedAdjustment,
			expectedSchedulerType,
			SchedulersDto(expectedSchedulers),
			list<RunTimeDto>(expectedRunTimes),
			list<StartTimeDto>(expectedStartTimes));

	EXPECT_FALSE(programDto.hasId());
	EXPECT_TRUE(programDto.hasEnabled());
	EXPECT_TRUE(programDto.hasName());
	EXPECT_TRUE(programDto.hasAdjustment());
	EXPECT_TRUE(programDto.hasSchedulerType());
	EXPECT_TRUE(programDto.hasSchedulers());
	EXPECT_TRUE(programDto.hasRunTimes());
	EXPECT_TRUE(programDto.hasStartTimes());

	EXPECT_THAT(programDto.getEnabled(), Eq(expectedEnabled));
	EXPECT_THAT(programDto.getName(), Eq(expectedName));
	EXPECT_THAT(programDto.getAdjustment(), Eq(expectedAdjustment));
	EXPECT_THAT(programDto.getSchedulerType(), Eq(expectedSchedulerType));
	EXPECT_THAT(programDto.getSchedulers(), Eq(expectedSchedulers));
	EXPECT_THAT(programDto.getRunTimes(), ContainerEq(expectedRunTimes));
	EXPECT_THAT(programDto.getStartTimes(), ContainerEq(expectedStartTimes));
}

TEST(ProgramDtoTest, copyConstructor) {
	const bool expectedEnabled = true;
	const string expectedName("name");
	const unsigned expectedAdjustment = 85;
	const string expectedSchedulerType("scheduler");
	const SchedulersDto expectedSchedulers(
			EveryDaySchedulerDto(),
			HotWeatherSchedulerDto(90, 31.0f),
			TemperatureDependentSchedulerDto(2.1f, 180, 190),
			WeeklySchedulerDto(list<bool>({ true, false, true }))
		);
	const list<RunTimeDto> expectedRunTimes({ RunTimeDto(0, 90), RunTimeDto(5, 0) });
	const list<StartTimeDto> expectedStartTimes({ StartTimeDto(1, 30), StartTimeDto(4, 0), StartTimeDto(6, 0) });

	const ProgramDto source(
			expectedEnabled,
			expectedName,
			expectedAdjustment,
			expectedSchedulerType,
			SchedulersDto(expectedSchedulers),
			list<RunTimeDto>(expectedRunTimes),
			list<StartTimeDto>(expectedStartTimes));

	const ProgramDto programDto(source);

	EXPECT_FALSE(programDto.hasId());
	EXPECT_TRUE(programDto.hasEnabled());
	EXPECT_TRUE(programDto.hasName());
	EXPECT_TRUE(programDto.hasAdjustment());
	EXPECT_TRUE(programDto.hasSchedulerType());
	EXPECT_TRUE(programDto.hasSchedulers());
	EXPECT_TRUE(programDto.hasRunTimes());
	EXPECT_TRUE(programDto.hasStartTimes());

	EXPECT_THAT(programDto.getEnabled(), Eq(expectedEnabled));
	EXPECT_THAT(programDto.getName(), Eq(expectedName));
	EXPECT_THAT(programDto.getAdjustment(), Eq(expectedAdjustment));
	EXPECT_THAT(programDto.getSchedulerType(), Eq(expectedSchedulerType));
	EXPECT_THAT(programDto.getSchedulers(), Eq(expectedSchedulers));
	EXPECT_THAT(programDto.getRunTimes(), ContainerEq(expectedRunTimes));
	EXPECT_THAT(programDto.getStartTimes(), ContainerEq(expectedStartTimes));
}

TEST(ProgramDtoTest, moveConstructor) {
	const bool expectedEnabled = false;
	const string expectedName("name");
	const unsigned expectedAdjustment = 93;
	const string expectedSchedulerType("scheduler");
	const SchedulersDto expectedSchedulers(
			EveryDaySchedulerDto(),
			HotWeatherSchedulerDto(43, 15.0f),
			TemperatureDependentSchedulerDto(1.1f, 63, 64),
			WeeklySchedulerDto(list<bool>({ true, false, true }))
		);
	const list<RunTimeDto> expectedRunTimes({ RunTimeDto(0, 90), RunTimeDto(5, 0) });
	const list<StartTimeDto> expectedStartTimes({ StartTimeDto(1, 30), StartTimeDto(4, 0), StartTimeDto(6, 0) });

	ProgramDto source(
			expectedEnabled,
			expectedName,
			expectedAdjustment,
			expectedSchedulerType,
			SchedulersDto(expectedSchedulers),
			list<RunTimeDto>(expectedRunTimes),
			list<StartTimeDto>(expectedStartTimes));

	const ProgramDto programDto(move(source));

	EXPECT_FALSE(programDto.hasId());
	EXPECT_TRUE(programDto.hasEnabled());
	EXPECT_TRUE(programDto.hasName());
	EXPECT_TRUE(programDto.hasAdjustment());
	EXPECT_TRUE(programDto.hasSchedulerType());
	EXPECT_TRUE(programDto.hasSchedulers());
	EXPECT_TRUE(programDto.hasRunTimes());
	EXPECT_TRUE(programDto.hasStartTimes());

	EXPECT_THAT(programDto.getEnabled(), Eq(expectedEnabled));
	EXPECT_THAT(programDto.getName(), Eq(expectedName));
	EXPECT_THAT(programDto.getAdjustment(), Eq(expectedAdjustment));
	EXPECT_THAT(programDto.getSchedulerType(), Eq(expectedSchedulerType));
	EXPECT_THAT(programDto.getSchedulers(), Eq(expectedSchedulers));
	EXPECT_THAT(programDto.getRunTimes(), ContainerEq(expectedRunTimes));
	EXPECT_THAT(programDto.getStartTimes(), ContainerEq(expectedStartTimes));
}

///////////////////////////////////////////////////////////////////////////////

CHECK_DTO_FUNCTIONS(ProgramDto, unsigned, Id, 135);
CHECK_DTO_FUNCTIONS(ProgramDto, bool, Enabled, false);
CHECK_DTO_FUNCTIONS(ProgramDto, string, Name, "123456");
CHECK_DTO_FUNCTIONS(ProgramDto, unsigned, Adjustment, 123);
CHECK_DTO_FUNCTIONS(ProgramDto, string, SchedulerType, "qwertzuio");
CHECK_DTO_FUNCTIONS(ProgramDto, SchedulersDto, Schedulers, SchedulersDto());
CHECK_DTO_FUNCTIONS(ProgramDto, std::list<RunTimeDto>, RunTimes, list<RunTimeDto>({ RunTimeDto(11, 10), RunTimeDto(11, 55), RunTimeDto(12, 0) }));
CHECK_DTO_FUNCTIONS(ProgramDto, std::list<StartTimeDto>, StartTimes, list<StartTimeDto>({ StartTimeDto(21, 20), StartTimeDto(22, 40), StartTimeDto(1, 23) }));

///////////////////////////////////////////////////////////////////////////////

TEST(ProgramDtoTest, equalsOperator) {
	ProgramDto dto1;
	ProgramDto dto2;

	EXPECT_TRUE(dto1 == dto2);
	EXPECT_TRUE(dto2 == dto1);

	CHECK_DTO_EQUALS_COPY(unsigned, Id, 5131, 444);
	CHECK_DTO_EQUALS_COPY(bool, Enabled, false, true);
	CHECK_DTO_EQUALS_COPY(string, Name, "abc", "123");
	CHECK_DTO_EQUALS_COPY(unsigned, Adjustment, 98, 63);
	CHECK_DTO_EQUALS_COPY(string, SchedulerType, "987654321", "schedulerType");

	CHECK_DTO_EQUALS_MOVE(SchedulersDto, Schedulers,
			SchedulersDto(
					EveryDaySchedulerDto(),
					HotWeatherSchedulerDto(90, 31.0f),
					TemperatureDependentSchedulerDto(2.1f, 180, 190),
					WeeklySchedulerDto(list<bool>({ true, false, true }))
				),
			SchedulersDto(
					EveryDaySchedulerDto(),
					HotWeatherSchedulerDto(43, 15.0f),
					TemperatureDependentSchedulerDto(1.1f, 63, 64),
					WeeklySchedulerDto(list<bool>({ true, false, true }))
				)

		);
	CHECK_DTO_EQUALS_MOVE(list<RunTimeDto>, RunTimes,
			list<RunTimeDto>({ RunTimeDto(10, 11), RunTimeDto(20, 21) }),
			list<RunTimeDto>({ RunTimeDto(12, 13), RunTimeDto(22, 23), RunTimeDto(32, 33) })
		);
	CHECK_DTO_EQUALS_MOVE(list<StartTimeDto>, StartTimes,
			list<StartTimeDto>({ StartTimeDto(17, 23), StartTimeDto(4, 26), StartTimeDto(5, 12) }),
			list<StartTimeDto>({ StartTimeDto(16, 17), StartTimeDto(18, 19), StartTimeDto(20, 21) })
		);
}
