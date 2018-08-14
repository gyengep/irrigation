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
	EXPECT_FALSE(programDTO.hasSpecifiedScheduler());
	EXPECT_FALSE(programDTO.hasRunTimes());
	EXPECT_FALSE(programDTO.hasStartTimes());
}

TEST(ProgramDTOTest, constructor) {
	const string expectedName("name");
	const string expectedSchedulerType("scheduler");
	const SpecifiedSchedulerDTO expectedSpecifiedScheduler(100, new list<bool>({ true, false, true }));
	const list<RunTimeDTO> expectedRunTimes({ RunTimeDTO(10, 0), RunTimeDTO(15, 0), RunTimeDTO(20, 0) });
	const list<StartTimeDTO> expectedStartTimes({ StartTimeDTO(5, 0), StartTimeDTO(6, 0) });

	ProgramDTO programDTO(
			expectedName,
			expectedSchedulerType,
			expectedSpecifiedScheduler,
			new list<RunTimeDTO>(expectedRunTimes),
			new list<StartTimeDTO>(expectedStartTimes));

	EXPECT_FALSE(programDTO.hasId());
	EXPECT_TRUE(programDTO.hasName());
	EXPECT_TRUE(programDTO.hasSchedulerType());
	EXPECT_TRUE(programDTO.hasSpecifiedScheduler());
	EXPECT_TRUE(programDTO.hasRunTimes());
	EXPECT_TRUE(programDTO.hasStartTimes());

	EXPECT_THAT(programDTO.getName(), Eq(expectedName));
	EXPECT_THAT(programDTO.getSchedulerType(), Eq(expectedSchedulerType));
	EXPECT_THAT(programDTO.getSpecifiedScheduler(), Eq(expectedSpecifiedScheduler));
	EXPECT_THAT(programDTO.getRunTimes(), ContainerEq(expectedRunTimes));
	EXPECT_THAT(programDTO.getStartTimes(), ContainerEq(expectedStartTimes));
}

TEST(ProgramDTOTest, copyConstructor) {
	const string expectedName("name");
	const string expectedSchedulerType("scheduler");
	const SpecifiedSchedulerDTO expectedSpecifiedScheduler(110, new list<bool>({ true, false, true }));
	const list<RunTimeDTO> expectedRunTimes({ RunTimeDTO(0, 90), RunTimeDTO(5, 0) });
	const list<StartTimeDTO> expectedStartTimes({ StartTimeDTO(1, 30), StartTimeDTO(4, 0), StartTimeDTO(6, 0) });

	const ProgramDTO other(
			expectedName,
			expectedSchedulerType,
			expectedSpecifiedScheduler,
			new list<RunTimeDTO>(expectedRunTimes),
			new list<StartTimeDTO>(expectedStartTimes));

	const ProgramDTO programDTO(other);

	EXPECT_FALSE(programDTO.hasId());
	EXPECT_TRUE(programDTO.hasName());
	EXPECT_TRUE(programDTO.hasSchedulerType());
	EXPECT_TRUE(programDTO.hasSpecifiedScheduler());
	EXPECT_TRUE(programDTO.hasRunTimes());
	EXPECT_TRUE(programDTO.hasStartTimes());

	EXPECT_THAT(programDTO.getName(), Eq(expectedName));
	EXPECT_THAT(programDTO.getSchedulerType(), Eq(expectedSchedulerType));
	EXPECT_THAT(programDTO.getSpecifiedScheduler(), Eq(expectedSpecifiedScheduler));
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
	programDTO.setName(expectedName.c_str());
	EXPECT_TRUE(programDTO.hasName());
}

TEST(ProgramDTOTest, getName) {
	const string expectedName("123456789");
	ProgramDTO programDTO;

	EXPECT_THROW(programDTO.getName(), logic_error);
	programDTO.setName(expectedName.c_str());
	ASSERT_NO_THROW(programDTO.getName());
	EXPECT_THAT(programDTO.getName(), Eq(expectedName));
}

TEST(ProgramDTOTest, hasSchedulerType) {
	const string expectedSchedulerType("qwertzuio");
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasSchedulerType());
	programDTO.setSchedulerType(expectedSchedulerType.c_str());
	EXPECT_TRUE(programDTO.hasSchedulerType());
}

TEST(ProgramDTOTest, getSchedulerType) {
	const string expectedSchedulerType("qwertzuio");
	ProgramDTO programDTO;

	EXPECT_THROW(programDTO.getSchedulerType(), logic_error);
	programDTO.setSchedulerType(expectedSchedulerType.c_str());
	ASSERT_NO_THROW(programDTO.getSchedulerType());
	EXPECT_THAT(programDTO.getSchedulerType(), Eq(expectedSchedulerType));
}

TEST(ProgramDTOTest, hasSpecifiedScheduler) {
	const SpecifiedSchedulerDTO expectedScheduler(90, new list<bool>());
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasSpecifiedScheduler());
	programDTO.setSpecifiedScheduler(expectedScheduler);
	EXPECT_TRUE(programDTO.hasSpecifiedScheduler());
}

TEST(ProgramDTOTest, getSpecifiedScheduler) {
	const SpecifiedSchedulerDTO expectedScheduler(80, new list<bool>({ true, false, true, false }));
	ProgramDTO programDTO;

	EXPECT_THROW(programDTO.getSpecifiedScheduler(), logic_error);
	programDTO.setSpecifiedScheduler(expectedScheduler);
	ASSERT_NO_THROW(programDTO.getSpecifiedScheduler());
	EXPECT_THAT(programDTO.getSpecifiedScheduler(), Eq(expectedScheduler));
}

TEST(ProgramDTOTest, hasRunTimes) {
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasRunTimes());
	programDTO.setRunTimes(new list<RunTimeDTO>());
	EXPECT_TRUE(programDTO.hasRunTimes());
}

TEST(ProgramDTOTest, getRunTimes) {
	const list<RunTimeDTO> expectedRunTimes({ RunTimeDTO(11, 10), RunTimeDTO(11, 55), RunTimeDTO(12, 0) });
	ProgramDTO programDTO;

	EXPECT_THROW(programDTO.getRunTimes(), logic_error);
	programDTO.setRunTimes(new list<RunTimeDTO>(expectedRunTimes));
	ASSERT_NO_THROW(programDTO.getRunTimes());
	EXPECT_THAT(programDTO.getRunTimes(), ContainerEq(expectedRunTimes));
}

TEST(ProgramDTOTest, hasStartTimes) {
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasStartTimes());
	programDTO.setStartTimes(new list<StartTimeDTO>());
	EXPECT_TRUE(programDTO.hasStartTimes());
}

TEST(ProgramDTOTest, getStartTimes) {
	list<StartTimeDTO> expectedStartTimes({ StartTimeDTO(21, 20), StartTimeDTO(22, 40), StartTimeDTO(1, 23) });
	ProgramDTO programDTO;

	EXPECT_THROW(programDTO.getStartTimes(), logic_error);
	programDTO.setStartTimes(new list<StartTimeDTO>(expectedStartTimes));
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

	programDTO1.setName(expectedName1.c_str());
	EXPECT_FALSE(programDTO1 == programDTO2);
	EXPECT_FALSE(programDTO2 == programDTO1);

	programDTO2.setName(expectedName2.c_str());
	EXPECT_FALSE(programDTO1 == programDTO2);
	EXPECT_FALSE(programDTO2 == programDTO1);

	programDTO1.setName(expectedName2.c_str());
	EXPECT_TRUE(programDTO1 == programDTO2);
	EXPECT_TRUE(programDTO2 == programDTO1);

	const string expectedSchedulerType1("987654321");
	const string expectedSchedulerType2("schedulerType");

	programDTO1.setSchedulerType(expectedSchedulerType1.c_str());
	EXPECT_FALSE(programDTO1 == programDTO2);
	EXPECT_FALSE(programDTO2 == programDTO1);

	programDTO2.setSchedulerType(expectedSchedulerType2.c_str());
	EXPECT_FALSE(programDTO1 == programDTO2);
	EXPECT_FALSE(programDTO2 == programDTO1);

	programDTO1.setSchedulerType(expectedSchedulerType2.c_str());
	EXPECT_TRUE(programDTO1 == programDTO2);
	EXPECT_TRUE(programDTO2 == programDTO1);

	const SpecifiedSchedulerDTO expectedScheduler1(20, new list<bool>({ true, true, false }));
	const SpecifiedSchedulerDTO expectedScheduler2(25, new list<bool>({ true, false, false }));

	programDTO1.setSpecifiedScheduler(expectedScheduler1);
	EXPECT_FALSE(programDTO1 == programDTO2);
	EXPECT_FALSE(programDTO2 == programDTO1);

	programDTO2.setSpecifiedScheduler(expectedScheduler2);
	EXPECT_FALSE(programDTO1 == programDTO2);
	EXPECT_FALSE(programDTO2 == programDTO1);

	programDTO1.setSpecifiedScheduler(expectedScheduler2);
	EXPECT_TRUE(programDTO1 == programDTO2);
	EXPECT_TRUE(programDTO2 == programDTO1);

	const list<RunTimeDTO> expectedRunTimes1({ RunTimeDTO(10, 11), RunTimeDTO(20, 21) });
	const list<RunTimeDTO> expectedRunTimes2({ RunTimeDTO(12, 13), RunTimeDTO(22, 23), RunTimeDTO(32, 33) });

	programDTO1.setRunTimes(new list<RunTimeDTO>(expectedRunTimes1));
	EXPECT_FALSE(programDTO1 == programDTO2);
	EXPECT_FALSE(programDTO2 == programDTO1);

	programDTO2.setRunTimes(new list<RunTimeDTO>(expectedRunTimes2));
	EXPECT_FALSE(programDTO1 == programDTO2);
	EXPECT_FALSE(programDTO2 == programDTO1);

	programDTO1.setRunTimes(new list<RunTimeDTO>(expectedRunTimes2));
	EXPECT_TRUE(programDTO1 == programDTO2);
	EXPECT_TRUE(programDTO2 == programDTO1);

	const list<StartTimeDTO> expectedStartTimes1({ StartTimeDTO(17, 23), StartTimeDTO(4, 26), StartTimeDTO(5, 12) });
	const list<StartTimeDTO> expectedStartTimes2({ StartTimeDTO(16, 17), StartTimeDTO(18, 19), StartTimeDTO(20, 21) });

	programDTO1.setStartTimes(new list<StartTimeDTO>(expectedStartTimes1));
	EXPECT_FALSE(programDTO1 == programDTO2);
	EXPECT_FALSE(programDTO2 == programDTO1);

	programDTO2.setStartTimes(new list<StartTimeDTO>(expectedStartTimes2));
	EXPECT_FALSE(programDTO1 == programDTO2);
	EXPECT_FALSE(programDTO2 == programDTO1);

	programDTO1.setStartTimes(new list<StartTimeDTO>(expectedStartTimes2));
	EXPECT_TRUE(programDTO1 == programDTO2);
	EXPECT_TRUE(programDTO2 == programDTO1);
}
