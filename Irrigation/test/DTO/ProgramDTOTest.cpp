#include <gmock/gmock.h>
#include <list>
#include <stdexcept>
#include "DTO/ProgramDTO.h"

using namespace std;
using namespace testing;



TEST(ProgramDTOTest, defaultConstructor) {
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasId());
	EXPECT_FALSE(programDTO.hasName());
	EXPECT_FALSE(programDTO.hasSchedulerType());
	EXPECT_FALSE(programDTO.hasPeriodicScheduler());
	EXPECT_FALSE(programDTO.hasWeeklyScheduler());
	EXPECT_FALSE(programDTO.hasRunTimes());
	EXPECT_FALSE(programDTO.hasStartTimes());
}

TEST(ProgramDTOTest, parametrizedConstructor) {
	const string expectedName("name");
	const string expectedSchedulerType("scheduler");
	const PeriodicSchedulerDTO expectedPeriodicScheduler(100, list<bool>({ true, false, true }), 2018, 11, 15);
	const WeeklySchedulerDTO expectedWeeklyScheduler(100, list<bool>({ true, false, true }));
	const list<RunTimeDTO> expectedRunTimes({ RunTimeDTO(10, 0), RunTimeDTO(15, 0), RunTimeDTO(20, 0) });
	const list<StartTimeDTO> expectedStartTimes({ StartTimeDTO(5, 0), StartTimeDTO(6, 0) });

	ProgramDTO programDTO(
			expectedName,
			expectedSchedulerType,
			PeriodicSchedulerDTO(expectedPeriodicScheduler),
			WeeklySchedulerDTO(expectedWeeklyScheduler),
			list<RunTimeDTO>(expectedRunTimes),
			list<StartTimeDTO>(expectedStartTimes));

	EXPECT_FALSE(programDTO.hasId());
	EXPECT_TRUE(programDTO.hasName());
	EXPECT_TRUE(programDTO.hasSchedulerType());
	EXPECT_TRUE(programDTO.hasPeriodicScheduler());
	EXPECT_TRUE(programDTO.hasWeeklyScheduler());
	EXPECT_TRUE(programDTO.hasRunTimes());
	EXPECT_TRUE(programDTO.hasStartTimes());

	EXPECT_THAT(programDTO.getName(), Eq(expectedName));
	EXPECT_THAT(programDTO.getSchedulerType(), Eq(expectedSchedulerType));
	EXPECT_THAT(programDTO.getPeriodicScheduler(), Eq(expectedPeriodicScheduler));
	EXPECT_THAT(programDTO.getWeeklyScheduler(), Eq(expectedWeeklyScheduler));
	EXPECT_THAT(programDTO.getRunTimes(), ContainerEq(expectedRunTimes));
	EXPECT_THAT(programDTO.getStartTimes(), ContainerEq(expectedStartTimes));
}

TEST(ProgramDTOTest, copyConstructor) {
	const string expectedName("name");
	const string expectedSchedulerType("scheduler");
	const PeriodicSchedulerDTO expectedPeriodicScheduler(100, list<bool>({ true, false, true }), 2018, 11, 15);
	const WeeklySchedulerDTO expectedWeeklyScheduler(110, list<bool>({ true, false, true }));
	const list<RunTimeDTO> expectedRunTimes({ RunTimeDTO(0, 90), RunTimeDTO(5, 0) });
	const list<StartTimeDTO> expectedStartTimes({ StartTimeDTO(1, 30), StartTimeDTO(4, 0), StartTimeDTO(6, 0) });

	const ProgramDTO source(
			expectedName,
			expectedSchedulerType,
			PeriodicSchedulerDTO(expectedPeriodicScheduler),
			WeeklySchedulerDTO(expectedWeeklyScheduler),
			list<RunTimeDTO>(expectedRunTimes),
			list<StartTimeDTO>(expectedStartTimes));

	ProgramDTO programDTO(source);

	EXPECT_FALSE(programDTO.hasId());
	EXPECT_TRUE(programDTO.hasName());
	EXPECT_TRUE(programDTO.hasSchedulerType());
	EXPECT_TRUE(programDTO.hasPeriodicScheduler());
	EXPECT_TRUE(programDTO.hasWeeklyScheduler());
	EXPECT_TRUE(programDTO.hasRunTimes());
	EXPECT_TRUE(programDTO.hasStartTimes());

	EXPECT_THAT(programDTO.getName(), Eq(expectedName));
	EXPECT_THAT(programDTO.getSchedulerType(), Eq(expectedSchedulerType));
	EXPECT_THAT(programDTO.getPeriodicScheduler(), Eq(expectedPeriodicScheduler));
	EXPECT_THAT(programDTO.getWeeklyScheduler(), Eq(expectedWeeklyScheduler));
	EXPECT_THAT(programDTO.getRunTimes(), ContainerEq(expectedRunTimes));
	EXPECT_THAT(programDTO.getStartTimes(), ContainerEq(expectedStartTimes));
}

TEST(ProgramDTOTest, moveConstructor) {
	const string expectedName("name");
	const string expectedSchedulerType("scheduler");
	const PeriodicSchedulerDTO expectedPeriodicScheduler(120, list<bool>({ false, true }), 2018, 11, 10);
	const WeeklySchedulerDTO expectedWeeklyScheduler(110, list<bool>({ true, false, true }));
	const list<RunTimeDTO> expectedRunTimes({ RunTimeDTO(0, 90), RunTimeDTO(5, 0) });
	const list<StartTimeDTO> expectedStartTimes({ StartTimeDTO(1, 30), StartTimeDTO(4, 0), StartTimeDTO(6, 0) });

	ProgramDTO source(
			expectedName,
			expectedSchedulerType,
			PeriodicSchedulerDTO(expectedPeriodicScheduler),
			WeeklySchedulerDTO(expectedWeeklyScheduler),
			list<RunTimeDTO>(expectedRunTimes),
			list<StartTimeDTO>(expectedStartTimes));

	ProgramDTO programDTO(move(source));

	EXPECT_FALSE(programDTO.hasId());
	EXPECT_TRUE(programDTO.hasName());
	EXPECT_TRUE(programDTO.hasSchedulerType());
	EXPECT_TRUE(programDTO.hasPeriodicScheduler());
	EXPECT_TRUE(programDTO.hasWeeklyScheduler());
	EXPECT_TRUE(programDTO.hasRunTimes());
	EXPECT_TRUE(programDTO.hasStartTimes());

	EXPECT_THAT(programDTO.getName(), Eq(expectedName));
	EXPECT_THAT(programDTO.getSchedulerType(), Eq(expectedSchedulerType));
	EXPECT_THAT(programDTO.getPeriodicScheduler(), Eq(expectedPeriodicScheduler));
	EXPECT_THAT(programDTO.getWeeklyScheduler(), Eq(expectedWeeklyScheduler));
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

TEST(ProgramDTOTest, hasName) {
	const string expectedName("khvikljb");
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasName());
	programDTO.setName(expectedName);
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

TEST(ProgramDTOTest, hasSchedulerType) {
	const string expectedSchedulerType("qwertzuio");
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasSchedulerType());
	programDTO.setSchedulerType(expectedSchedulerType);
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
	const PeriodicSchedulerDTO expectedScheduler(100, list<bool>({ true, false, true }), 2018, 11, 15);
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasPeriodicScheduler());
	programDTO.setPeriodicScheduler(PeriodicSchedulerDTO(expectedScheduler));
	EXPECT_TRUE(programDTO.hasPeriodicScheduler());
}

TEST(ProgramDTOTest, getPeriodicScheduler) {
	const PeriodicSchedulerDTO expectedScheduler(100, list<bool>({ true, false, true }), 2018, 11, 15);
	ProgramDTO programDTO;

	EXPECT_THROW(programDTO.getPeriodicScheduler(), logic_error);
	programDTO.setPeriodicScheduler(PeriodicSchedulerDTO(expectedScheduler));
	ASSERT_NO_THROW(programDTO.getPeriodicScheduler());
	EXPECT_THAT(programDTO.getPeriodicScheduler(), Eq(expectedScheduler));
}

TEST(ProgramDTOTest, hasWeeklyScheduler) {
	const WeeklySchedulerDTO expectedScheduler(90, list<bool>());
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasWeeklyScheduler());
	programDTO.setWeeklyScheduler(WeeklySchedulerDTO(expectedScheduler));
	EXPECT_TRUE(programDTO.hasWeeklyScheduler());
}

TEST(ProgramDTOTest, getWeeklyScheduler) {
	const WeeklySchedulerDTO expectedScheduler(80, list<bool>({ true, false, true, false }));
	ProgramDTO programDTO;

	EXPECT_THROW(programDTO.getWeeklyScheduler(), logic_error);
	programDTO.setWeeklyScheduler(WeeklySchedulerDTO(expectedScheduler));
	ASSERT_NO_THROW(programDTO.getWeeklyScheduler());
	EXPECT_THAT(programDTO.getWeeklyScheduler(), Eq(expectedScheduler));
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

TEST(ProgramDTOTest, equal) {
	ProgramDTO programDTO1;
	ProgramDTO programDTO2;

	EXPECT_TRUE(programDTO1 == programDTO2);
	EXPECT_TRUE(programDTO2 == programDTO1);

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

	const PeriodicSchedulerDTO expectedPeriodicScheduler1(120, list<bool>({ true, false, true }), 2018, 11, 15);
	const PeriodicSchedulerDTO expectedPeriodicScheduler2(80, list<bool>({ true, false, false }), 2015, 10, 15);

	programDTO1.setPeriodicScheduler(PeriodicSchedulerDTO(expectedPeriodicScheduler1));
	EXPECT_FALSE(programDTO1 == programDTO2);
	EXPECT_FALSE(programDTO2 == programDTO1);

	programDTO2.setPeriodicScheduler(PeriodicSchedulerDTO(expectedPeriodicScheduler2));
	EXPECT_FALSE(programDTO1 == programDTO2);
	EXPECT_FALSE(programDTO2 == programDTO1);

	programDTO1.setPeriodicScheduler(PeriodicSchedulerDTO(expectedPeriodicScheduler2));
	EXPECT_TRUE(programDTO1 == programDTO2);
	EXPECT_TRUE(programDTO2 == programDTO1);

	const WeeklySchedulerDTO expectedWeeklyScheduler1(20, list<bool>({ true, true, false }));
	const WeeklySchedulerDTO expectedWeeklyScheduler2(25, list<bool>({ true, false, false }));

	programDTO1.setWeeklyScheduler(WeeklySchedulerDTO(expectedWeeklyScheduler1));
	EXPECT_FALSE(programDTO1 == programDTO2);
	EXPECT_FALSE(programDTO2 == programDTO1);

	programDTO2.setWeeklyScheduler(WeeklySchedulerDTO(expectedWeeklyScheduler2));
	EXPECT_FALSE(programDTO1 == programDTO2);
	EXPECT_FALSE(programDTO2 == programDTO1);

	programDTO1.setWeeklyScheduler(WeeklySchedulerDTO(expectedWeeklyScheduler2));
	EXPECT_TRUE(programDTO1 == programDTO2);
	EXPECT_TRUE(programDTO2 == programDTO1);

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
