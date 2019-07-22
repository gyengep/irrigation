#include <gmock/gmock.h>
#include <list>
#include <stdexcept>
#include "DTO/ProgramDTO.h"

using namespace std;
using namespace testing;



TEST(ProgramDTOTest, defaultConstructor) {
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasId());
	EXPECT_FALSE(programDTO.hasDisabled());
	EXPECT_FALSE(programDTO.hasName());
	EXPECT_FALSE(programDTO.hasAdjustment());
	EXPECT_FALSE(programDTO.hasSchedulerType());
	EXPECT_FALSE(programDTO.hasPeriodicScheduler());
	EXPECT_FALSE(programDTO.hasWeeklyScheduler());
	EXPECT_FALSE(programDTO.hasRunTimes());
	EXPECT_FALSE(programDTO.hasStartTimes());
}

TEST(ProgramDTOTest, parametrizedConstructor) {
	const bool expectedDisabled = true;
	const string expectedName("name");
	const unsigned expectedAdjustment = 78;
	const string expectedSchedulerType("scheduler");
	const PeriodicSchedulerDTO expectedPeriodicScheduler(list<bool>({ true, false, true }), 2018, 11, 15);
	const WeeklySchedulerDTO expectedWeeklyScheduler(list<bool>({ true, false, true }));
	const EveryDaySchedulerDTO expectedEveryDayScheduler;
	const list<RunTimeDTO> expectedRunTimes({ RunTimeDTO(10, 0), RunTimeDTO(15, 0), RunTimeDTO(20, 0) });
	const list<StartTimeDTO> expectedStartTimes({ StartTimeDTO(5, 0), StartTimeDTO(6, 0) });

	ProgramDTO programDTO(
			expectedDisabled,
			expectedName,
			expectedAdjustment,
			expectedSchedulerType,
			PeriodicSchedulerDTO(expectedPeriodicScheduler),
			WeeklySchedulerDTO(expectedWeeklyScheduler),
			EveryDaySchedulerDTO(expectedEveryDayScheduler),
			list<RunTimeDTO>(expectedRunTimes),
			list<StartTimeDTO>(expectedStartTimes));

	EXPECT_FALSE(programDTO.hasId());
	EXPECT_TRUE(programDTO.hasDisabled());
	EXPECT_TRUE(programDTO.hasName());
	EXPECT_TRUE(programDTO.hasAdjustment());
	EXPECT_TRUE(programDTO.hasSchedulerType());
	EXPECT_TRUE(programDTO.hasPeriodicScheduler());
	EXPECT_TRUE(programDTO.hasWeeklyScheduler());
	EXPECT_TRUE(programDTO.hasEveryDayScheduler());
	EXPECT_TRUE(programDTO.hasRunTimes());
	EXPECT_TRUE(programDTO.hasStartTimes());

	EXPECT_THAT(programDTO.getDisabled(), Eq(expectedDisabled));
	EXPECT_THAT(programDTO.getName(), Eq(expectedName));
	EXPECT_THAT(programDTO.getAdjustment(), Eq(expectedAdjustment));
	EXPECT_THAT(programDTO.getSchedulerType(), Eq(expectedSchedulerType));
	EXPECT_THAT(programDTO.getPeriodicScheduler(), Eq(expectedPeriodicScheduler));
	EXPECT_THAT(programDTO.getWeeklyScheduler(), Eq(expectedWeeklyScheduler));
	EXPECT_THAT(programDTO.getEveryDayScheduler(), Eq(expectedEveryDayScheduler));
	EXPECT_THAT(programDTO.getRunTimes(), ContainerEq(expectedRunTimes));
	EXPECT_THAT(programDTO.getStartTimes(), ContainerEq(expectedStartTimes));
}

TEST(ProgramDTOTest, copyConstructor) {
	const bool expectedDisabled = false;
	const string expectedName("name");
	const unsigned expectedAdjustment = 85;
	const string expectedSchedulerType("scheduler");
	const PeriodicSchedulerDTO expectedPeriodicScheduler(list<bool>({ true, false, true }), 2018, 11, 15);
	const WeeklySchedulerDTO expectedWeeklyScheduler(list<bool>({ true, false, true }));
	const EveryDaySchedulerDTO expectedEveryDayScheduler;
	const list<RunTimeDTO> expectedRunTimes({ RunTimeDTO(0, 90), RunTimeDTO(5, 0) });
	const list<StartTimeDTO> expectedStartTimes({ StartTimeDTO(1, 30), StartTimeDTO(4, 0), StartTimeDTO(6, 0) });

	const ProgramDTO source(
			expectedDisabled,
			expectedName,
			expectedAdjustment,
			expectedSchedulerType,
			PeriodicSchedulerDTO(expectedPeriodicScheduler),
			WeeklySchedulerDTO(expectedWeeklyScheduler),
			EveryDaySchedulerDTO(expectedEveryDayScheduler),
			list<RunTimeDTO>(expectedRunTimes),
			list<StartTimeDTO>(expectedStartTimes));

	ProgramDTO programDTO(source);

	EXPECT_FALSE(programDTO.hasId());
	EXPECT_TRUE(programDTO.hasDisabled());
	EXPECT_TRUE(programDTO.hasName());
	EXPECT_TRUE(programDTO.hasAdjustment());
	EXPECT_TRUE(programDTO.hasSchedulerType());
	EXPECT_TRUE(programDTO.hasPeriodicScheduler());
	EXPECT_TRUE(programDTO.hasWeeklyScheduler());
	EXPECT_TRUE(programDTO.hasEveryDayScheduler());
	EXPECT_TRUE(programDTO.hasRunTimes());
	EXPECT_TRUE(programDTO.hasStartTimes());

	EXPECT_THAT(programDTO.getDisabled(), Eq(expectedDisabled));
	EXPECT_THAT(programDTO.getName(), Eq(expectedName));
	EXPECT_THAT(programDTO.getAdjustment(), Eq(expectedAdjustment));
	EXPECT_THAT(programDTO.getSchedulerType(), Eq(expectedSchedulerType));
	EXPECT_THAT(programDTO.getPeriodicScheduler(), Eq(expectedPeriodicScheduler));
	EXPECT_THAT(programDTO.getWeeklyScheduler(), Eq(expectedWeeklyScheduler));
	EXPECT_THAT(programDTO.getEveryDayScheduler(), Eq(expectedEveryDayScheduler));
	EXPECT_THAT(programDTO.getRunTimes(), ContainerEq(expectedRunTimes));
	EXPECT_THAT(programDTO.getStartTimes(), ContainerEq(expectedStartTimes));
}

TEST(ProgramDTOTest, moveConstructor) {
	const bool expectedDisabled = true;
	const string expectedName("name");
	const unsigned expectedAdjustment = 93;
	const string expectedSchedulerType("scheduler");
	const PeriodicSchedulerDTO expectedPeriodicScheduler(list<bool>({ false, true }), 2018, 11, 10);
	const WeeklySchedulerDTO expectedWeeklyScheduler(list<bool>({ true, false, true }));
	const EveryDaySchedulerDTO expectedEveryDayScheduler;
	const list<RunTimeDTO> expectedRunTimes({ RunTimeDTO(0, 90), RunTimeDTO(5, 0) });
	const list<StartTimeDTO> expectedStartTimes({ StartTimeDTO(1, 30), StartTimeDTO(4, 0), StartTimeDTO(6, 0) });

	ProgramDTO source(
			expectedDisabled,
			expectedName,
			expectedAdjustment,
			expectedSchedulerType,
			PeriodicSchedulerDTO(expectedPeriodicScheduler),
			WeeklySchedulerDTO(expectedWeeklyScheduler),
			EveryDaySchedulerDTO(expectedEveryDayScheduler),
			list<RunTimeDTO>(expectedRunTimes),
			list<StartTimeDTO>(expectedStartTimes));

	ProgramDTO programDTO(move(source));

	EXPECT_FALSE(programDTO.hasId());
	EXPECT_TRUE(programDTO.hasDisabled());
	EXPECT_TRUE(programDTO.hasName());
	EXPECT_TRUE(programDTO.hasAdjustment());
	EXPECT_TRUE(programDTO.hasSchedulerType());
	EXPECT_TRUE(programDTO.hasPeriodicScheduler());
	EXPECT_TRUE(programDTO.hasWeeklyScheduler());
	EXPECT_TRUE(programDTO.hasEveryDayScheduler());
	EXPECT_TRUE(programDTO.hasRunTimes());
	EXPECT_TRUE(programDTO.hasStartTimes());

	EXPECT_THAT(programDTO.getDisabled(), Eq(expectedDisabled));
	EXPECT_THAT(programDTO.getName(), Eq(expectedName));
	EXPECT_THAT(programDTO.getAdjustment(), Eq(expectedAdjustment));
	EXPECT_THAT(programDTO.getSchedulerType(), Eq(expectedSchedulerType));
	EXPECT_THAT(programDTO.getPeriodicScheduler(), Eq(expectedPeriodicScheduler));
	EXPECT_THAT(programDTO.getWeeklyScheduler(), Eq(expectedWeeklyScheduler));
	EXPECT_THAT(programDTO.getEveryDayScheduler(), Eq(expectedEveryDayScheduler));
	EXPECT_THAT(programDTO.getRunTimes(), ContainerEq(expectedRunTimes));
	EXPECT_THAT(programDTO.getStartTimes(), ContainerEq(expectedStartTimes));
}

TEST(ProgramDTOTest, hasId) {
	const unsigned expectedId = 135;
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasId());
	programDTO.setId(expectedId);
	EXPECT_TRUE(programDTO.hasId());
}

TEST(ProgramDTOTest, getId) {
	const unsigned expectedId = 2135;
	ProgramDTO programDTO;

	EXPECT_THROW(programDTO.getId(), logic_error);
	programDTO.setId(expectedId);
	ASSERT_NO_THROW(programDTO.getId());
	EXPECT_THAT(programDTO.getId(), expectedId);
}
TEST(ProgramDTOTest, hasDisabled) {
	const bool expectedDisabled = true;
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasDisabled());
	programDTO.setDisabled(expectedDisabled);
	EXPECT_TRUE(programDTO.hasDisabled());
}

TEST(ProgramDTOTest, getDisabled) {
	ProgramDTO programDTO;

	EXPECT_THROW(programDTO.getDisabled(), logic_error);
	programDTO.setDisabled(true);
	ASSERT_NO_THROW(programDTO.getDisabled());
	EXPECT_TRUE(programDTO.getDisabled());
	programDTO.setDisabled(false);
	ASSERT_NO_THROW(programDTO.getDisabled());
	EXPECT_FALSE(programDTO.getDisabled());
}

TEST(ProgramDTOTest, hasName) {
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasName());
	programDTO.setName(string("khvikljb"));
	EXPECT_TRUE(programDTO.hasName());
}

TEST(ProgramDTOTest, getName) {
	const string expectedName("123456789");
	ProgramDTO programDTO;

	EXPECT_THROW(programDTO.getName(), logic_error);
	programDTO.setName(expectedName);
	ASSERT_NO_THROW(programDTO.getName());
	EXPECT_THAT(programDTO.getName(), Eq(expectedName));
}

TEST(ProgramDTOTest, hasAdjustment) {
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasAdjustment());
	programDTO.setAdjustment(16);
	EXPECT_TRUE(programDTO.hasAdjustment());
}

TEST(ProgramDTOTest, getAdjustment) {
	const unsigned expectedAdjustment = 123;
	ProgramDTO programDTO;

	EXPECT_THROW(programDTO.getAdjustment(), logic_error);
	programDTO.setAdjustment(expectedAdjustment);
	ASSERT_NO_THROW(programDTO.getAdjustment());
	EXPECT_THAT(programDTO.getAdjustment(), Eq(expectedAdjustment));
}

TEST(ProgramDTOTest, hasSchedulerType) {
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasSchedulerType());
	programDTO.setSchedulerType(string("qwertzuio"));
	EXPECT_TRUE(programDTO.hasSchedulerType());
}

TEST(ProgramDTOTest, getSchedulerType) {
	const string expectedSchedulerType("qwertzuio");
	ProgramDTO programDTO;

	EXPECT_THROW(programDTO.getSchedulerType(), logic_error);
	programDTO.setSchedulerType(expectedSchedulerType);
	ASSERT_NO_THROW(programDTO.getSchedulerType());
	EXPECT_THAT(programDTO.getSchedulerType(), Eq(expectedSchedulerType));
}

TEST(ProgramDTOTest, hasPeriodicScheduler) {
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasPeriodicScheduler());
	programDTO.setPeriodicScheduler(PeriodicSchedulerDTO());
	EXPECT_TRUE(programDTO.hasPeriodicScheduler());
}

TEST(ProgramDTOTest, getPeriodicScheduler) {
	const PeriodicSchedulerDTO expectedScheduler(list<bool>({ true, false, true }), 2018, 11, 15);
	ProgramDTO programDTO;

	EXPECT_THROW(programDTO.getPeriodicScheduler(), logic_error);
	programDTO.setPeriodicScheduler(PeriodicSchedulerDTO(expectedScheduler));
	ASSERT_NO_THROW(programDTO.getPeriodicScheduler());
	EXPECT_THAT(programDTO.getPeriodicScheduler(), Eq(expectedScheduler));
}

TEST(ProgramDTOTest, hasWeeklyScheduler) {
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasWeeklyScheduler());
	programDTO.setWeeklyScheduler(WeeklySchedulerDTO());
	EXPECT_TRUE(programDTO.hasWeeklyScheduler());
}

TEST(ProgramDTOTest, getWeeklyScheduler) {
	const WeeklySchedulerDTO expectedScheduler(list<bool>({ true, false, true, false }));
	ProgramDTO programDTO;

	EXPECT_THROW(programDTO.getWeeklyScheduler(), logic_error);
	programDTO.setWeeklyScheduler(WeeklySchedulerDTO(expectedScheduler));
	ASSERT_NO_THROW(programDTO.getWeeklyScheduler());
	EXPECT_THAT(programDTO.getWeeklyScheduler(), Eq(expectedScheduler));
}

TEST(ProgramDTOTest, hasEveryDayScheduler) {
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasEveryDayScheduler());
	programDTO.setEveryDayScheduler(EveryDaySchedulerDTO());
	EXPECT_TRUE(programDTO.hasEveryDayScheduler());
}

TEST(ProgramDTOTest, getEveryDayScheduler) {
	const EveryDaySchedulerDTO expectedScheduler;
	ProgramDTO programDTO;

	EXPECT_THROW(programDTO.getEveryDayScheduler(), logic_error);
	programDTO.setEveryDayScheduler(EveryDaySchedulerDTO(expectedScheduler));
	ASSERT_NO_THROW(programDTO.getEveryDayScheduler());
	EXPECT_THAT(programDTO.getEveryDayScheduler(), Eq(expectedScheduler));
}

TEST(ProgramDTOTest, hasRunTimes) {
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasRunTimes());
	programDTO.setRunTimes(list<RunTimeDTO>());
	EXPECT_TRUE(programDTO.hasRunTimes());
}

TEST(ProgramDTOTest, getRunTimes) {
	const list<RunTimeDTO> expectedRunTimes({ RunTimeDTO(11, 10), RunTimeDTO(11, 55), RunTimeDTO(12, 0) });
	ProgramDTO programDTO;

	EXPECT_THROW(programDTO.getRunTimes(), logic_error);
	programDTO.setRunTimes(list<RunTimeDTO>(expectedRunTimes));
	ASSERT_NO_THROW(programDTO.getRunTimes());
	EXPECT_THAT(programDTO.getRunTimes(), ContainerEq(expectedRunTimes));
}

TEST(ProgramDTOTest, hasStartTimes) {
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasStartTimes());
	programDTO.setStartTimes(list<StartTimeDTO>());
	EXPECT_TRUE(programDTO.hasStartTimes());
}

TEST(ProgramDTOTest, getStartTimes) {
	list<StartTimeDTO> expectedStartTimes({ StartTimeDTO(21, 20), StartTimeDTO(22, 40), StartTimeDTO(1, 23) });
	ProgramDTO programDTO;

	EXPECT_THROW(programDTO.getStartTimes(), logic_error);
	programDTO.setStartTimes(list<StartTimeDTO>(expectedStartTimes));
	ASSERT_NO_THROW(programDTO.getStartTimes());
	EXPECT_THAT(programDTO.getStartTimes(), ContainerEq(expectedStartTimes));
}

TEST(ProgramDTOTest, equalsOperator) {
	ProgramDTO programDTO1;
	ProgramDTO programDTO2;

	EXPECT_TRUE(programDTO1 == programDTO2);
	EXPECT_TRUE(programDTO2 == programDTO1);

	{
		const unsigned expectedId1 = 5131;
		const unsigned expectedId2 = 444;

		programDTO1.setId(expectedId1);
		EXPECT_FALSE(programDTO1 == programDTO2);
		EXPECT_FALSE(programDTO2 == programDTO1);

		programDTO2.setId(expectedId2);
		EXPECT_FALSE(programDTO1 == programDTO2);
		EXPECT_FALSE(programDTO2 == programDTO1);

		programDTO1.setId(expectedId2);
		EXPECT_TRUE(programDTO1 == programDTO2);
		EXPECT_TRUE(programDTO2 == programDTO1);
	}

	{
		const bool expectedDisabled1 = true;
		const bool expectedDisabled2 = false;

		programDTO1.setDisabled(expectedDisabled1);
		EXPECT_FALSE(programDTO1 == programDTO2);
		EXPECT_FALSE(programDTO2 == programDTO1);

		programDTO2.setDisabled(expectedDisabled2);
		EXPECT_FALSE(programDTO1 == programDTO2);
		EXPECT_FALSE(programDTO2 == programDTO1);

		programDTO1.setDisabled(expectedDisabled2);
		EXPECT_TRUE(programDTO1 == programDTO2);
		EXPECT_TRUE(programDTO2 == programDTO1);
	}

	{
		const string expectedName1("1234");
		const string expectedName2("QWERTZUIO");

		programDTO1.setName(expectedName1);
		EXPECT_FALSE(programDTO1 == programDTO2);
		EXPECT_FALSE(programDTO2 == programDTO1);

		programDTO2.setName(expectedName2);
		EXPECT_FALSE(programDTO1 == programDTO2);
		EXPECT_FALSE(programDTO2 == programDTO1);

		programDTO1.setName(expectedName2);
		EXPECT_TRUE(programDTO1 == programDTO2);
		EXPECT_TRUE(programDTO2 == programDTO1);
	}

	{
		const unsigned expectedAdjustment1 = 98;
		const unsigned expectedAdjustment2 = 63;

		programDTO1.setAdjustment(expectedAdjustment1);
		EXPECT_FALSE(programDTO1 == programDTO2);
		EXPECT_FALSE(programDTO2 == programDTO1);

		programDTO2.setAdjustment(expectedAdjustment2);
		EXPECT_FALSE(programDTO1 == programDTO2);
		EXPECT_FALSE(programDTO2 == programDTO1);

		programDTO1.setAdjustment(expectedAdjustment2);
		EXPECT_TRUE(programDTO1 == programDTO2);
		EXPECT_TRUE(programDTO2 == programDTO1);
	}

	{
		const string expectedSchedulerType1("987654321");
		const string expectedSchedulerType2("schedulerType");

		programDTO1.setSchedulerType(expectedSchedulerType1);
		EXPECT_FALSE(programDTO1 == programDTO2);
		EXPECT_FALSE(programDTO2 == programDTO1);

		programDTO2.setSchedulerType(expectedSchedulerType2);
		EXPECT_FALSE(programDTO1 == programDTO2);
		EXPECT_FALSE(programDTO2 == programDTO1);

		programDTO1.setSchedulerType(expectedSchedulerType2);
		EXPECT_TRUE(programDTO1 == programDTO2);
		EXPECT_TRUE(programDTO2 == programDTO1);
	}

	{
		const PeriodicSchedulerDTO expectedPeriodicScheduler1(list<bool>({ true, false, true }), 2018, 11, 15);
		const PeriodicSchedulerDTO expectedPeriodicScheduler2(list<bool>({ true, false, false }), 2015, 10, 15);

		programDTO1.setPeriodicScheduler(PeriodicSchedulerDTO(expectedPeriodicScheduler1));
		EXPECT_FALSE(programDTO1 == programDTO2);
		EXPECT_FALSE(programDTO2 == programDTO1);

		programDTO2.setPeriodicScheduler(PeriodicSchedulerDTO(expectedPeriodicScheduler2));
		EXPECT_FALSE(programDTO1 == programDTO2);
		EXPECT_FALSE(programDTO2 == programDTO1);

		programDTO1.setPeriodicScheduler(PeriodicSchedulerDTO(expectedPeriodicScheduler2));
		EXPECT_TRUE(programDTO1 == programDTO2);
		EXPECT_TRUE(programDTO2 == programDTO1);
	}

	{
		const WeeklySchedulerDTO expectedWeeklyScheduler1(list<bool>({ true, true, false }));
		const WeeklySchedulerDTO expectedWeeklyScheduler2(list<bool>({ true, false, false }));

		programDTO1.setWeeklyScheduler(WeeklySchedulerDTO(expectedWeeklyScheduler1));
		EXPECT_FALSE(programDTO1 == programDTO2);
		EXPECT_FALSE(programDTO2 == programDTO1);

		programDTO2.setWeeklyScheduler(WeeklySchedulerDTO(expectedWeeklyScheduler2));
		EXPECT_FALSE(programDTO1 == programDTO2);
		EXPECT_FALSE(programDTO2 == programDTO1);

		programDTO1.setWeeklyScheduler(WeeklySchedulerDTO(expectedWeeklyScheduler2));
		EXPECT_TRUE(programDTO1 == programDTO2);
		EXPECT_TRUE(programDTO2 == programDTO1);
	}

	{
		const list<RunTimeDTO> expectedRunTimes1({ RunTimeDTO(10, 11), RunTimeDTO(20, 21) });
		const list<RunTimeDTO> expectedRunTimes2({ RunTimeDTO(12, 13), RunTimeDTO(22, 23), RunTimeDTO(32, 33) });

		programDTO1.setRunTimes(list<RunTimeDTO>(expectedRunTimes1));
		EXPECT_FALSE(programDTO1 == programDTO2);
		EXPECT_FALSE(programDTO2 == programDTO1);

		programDTO2.setRunTimes(list<RunTimeDTO>(expectedRunTimes2));
		EXPECT_FALSE(programDTO1 == programDTO2);
		EXPECT_FALSE(programDTO2 == programDTO1);

		programDTO1.setRunTimes(list<RunTimeDTO>(expectedRunTimes2));
		EXPECT_TRUE(programDTO1 == programDTO2);
		EXPECT_TRUE(programDTO2 == programDTO1);
	}

	{
		const list<StartTimeDTO> expectedStartTimes1({ StartTimeDTO(17, 23), StartTimeDTO(4, 26), StartTimeDTO(5, 12) });
		const list<StartTimeDTO> expectedStartTimes2({ StartTimeDTO(16, 17), StartTimeDTO(18, 19), StartTimeDTO(20, 21) });

		programDTO1.setStartTimes(list<StartTimeDTO>(expectedStartTimes1));
		EXPECT_FALSE(programDTO1 == programDTO2);
		EXPECT_FALSE(programDTO2 == programDTO1);

		programDTO2.setStartTimes(list<StartTimeDTO>(expectedStartTimes2));
		EXPECT_FALSE(programDTO1 == programDTO2);
		EXPECT_FALSE(programDTO2 == programDTO1);

		programDTO1.setStartTimes(list<StartTimeDTO>(expectedStartTimes2));
		EXPECT_TRUE(programDTO1 == programDTO2);
		EXPECT_TRUE(programDTO2 == programDTO1);
	}
}
