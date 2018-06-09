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
	ProgramDTO programDTO("name", "scheduler",
			SpecifiedSchedulerDTO(new list<bool>({ true, false, true })),
			new list<RunTimeDTO>({ RunTimeDTO(10), RunTimeDTO(15), RunTimeDTO(20) }),
			new list<StartTimeDTO>({ StartTimeDTO(31), StartTimeDTO(18) }));

	EXPECT_FALSE(programDTO.hasId());
	EXPECT_TRUE(programDTO.hasName());
	EXPECT_TRUE(programDTO.hasSchedulerType());
	EXPECT_TRUE(programDTO.hasSpecifiedScheduler());
	EXPECT_TRUE(programDTO.hasRunTimes());
	EXPECT_TRUE(programDTO.hasStartTimes());

	EXPECT_THAT(programDTO.getName().c_str(), StrEq("name"));
	EXPECT_THAT(programDTO.getSchedulerType().c_str(), StrEq("scheduler"));
	EXPECT_THAT(programDTO.getSpecifiedScheduler(), SpecifiedSchedulerDTO(new list<bool>({ true, false, true })));
	EXPECT_THAT(programDTO.getRunTimes(), ContainerEq(list<RunTimeDTO>({ RunTimeDTO(10), RunTimeDTO(15), RunTimeDTO(20) })));
	EXPECT_THAT(programDTO.getStartTimes(), ContainerEq(list<StartTimeDTO>({ StartTimeDTO(31), StartTimeDTO(18) })));
}

TEST(ProgramDTOTest, copyConstructor) {
	ProgramDTO programDTO(ProgramDTO("name", "scheduler",
			SpecifiedSchedulerDTO(new list<bool>({ true, false, true })),
			new list<RunTimeDTO>({ RunTimeDTO(10), RunTimeDTO(15), RunTimeDTO(20) }),
			new list<StartTimeDTO>({ StartTimeDTO(31), StartTimeDTO(18) })));

	EXPECT_FALSE(programDTO.hasId());
	EXPECT_TRUE(programDTO.hasName());
	EXPECT_TRUE(programDTO.hasSchedulerType());
	EXPECT_TRUE(programDTO.hasSpecifiedScheduler());
	EXPECT_TRUE(programDTO.hasRunTimes());
	EXPECT_TRUE(programDTO.hasStartTimes());

	EXPECT_THAT(programDTO.getName().c_str(), StrEq("name"));
	EXPECT_THAT(programDTO.getSchedulerType().c_str(), StrEq("scheduler"));
	EXPECT_THAT(programDTO.getSpecifiedScheduler(), SpecifiedSchedulerDTO(new list<bool>({ true, false, true })));
	EXPECT_THAT(programDTO.getRunTimes(), ContainerEq(list<RunTimeDTO>({ RunTimeDTO(10), RunTimeDTO(15), RunTimeDTO(20) })));
	EXPECT_THAT(programDTO.getStartTimes(), ContainerEq(list<StartTimeDTO>({ StartTimeDTO(31), StartTimeDTO(18) })));
}

TEST(ProgramDTOTest, getId) {
	ProgramDTO programDTO;

	EXPECT_THROW(programDTO.getId(), logic_error);
	programDTO.setId(5);
	ASSERT_NO_THROW(programDTO.getId());
	EXPECT_THAT(programDTO.getId(), 5);
}

TEST(ProgramDTOTest, getName) {
	ProgramDTO programDTO;

	EXPECT_THROW(programDTO.getName(), logic_error);
	programDTO.setName("abcd");
	ASSERT_NO_THROW(programDTO.getName());
	EXPECT_THAT(programDTO.getName().c_str(), StrEq("abcd"));
}

TEST(ProgramDTOTest, getSchedulerType) {
	ProgramDTO programDTO;

	EXPECT_THROW(programDTO.getSchedulerType(), logic_error);
	programDTO.setSchedulerType("test_scheduler");
	ASSERT_NO_THROW(programDTO.getSchedulerType());
	EXPECT_THAT(programDTO.getSchedulerType().c_str(), StrEq("test_scheduler"));
}

TEST(ProgramDTOTest, getSpecifiedScheduler) {
	ProgramDTO programDTO;

	EXPECT_THROW(programDTO.getSpecifiedScheduler(), logic_error);
	programDTO.setSpecifiedScheduler(SpecifiedSchedulerDTO(new list<bool>({ true, false })));
	ASSERT_NO_THROW(programDTO.getSpecifiedScheduler());
	EXPECT_THAT(programDTO.getSpecifiedScheduler(), SpecifiedSchedulerDTO(new list<bool>({ true, false })));
}

TEST(ProgramDTOTest, getRunTimes) {
	ProgramDTO programDTO;

	EXPECT_THROW(programDTO.getRunTimes(), logic_error);
	programDTO.setRunTimes(new list<RunTimeDTO>({ RunTimeDTO(100), RunTimeDTO(120) }));
	ASSERT_NO_THROW(programDTO.getRunTimes());
	EXPECT_THAT(programDTO.getRunTimes(), ContainerEq(list<RunTimeDTO>({ RunTimeDTO(100), RunTimeDTO(120) })));
}

TEST(ProgramDTOTest, getStartTimes) {
	ProgramDTO programDTO;

	EXPECT_THROW(programDTO.getStartTimes(), logic_error);
	programDTO.setStartTimes(new list<StartTimeDTO>({ StartTimeDTO(100), StartTimeDTO(120) }));
	ASSERT_NO_THROW(programDTO.getStartTimes());
	EXPECT_THAT(programDTO.getStartTimes(), ContainerEq(list<StartTimeDTO>({ StartTimeDTO(100), StartTimeDTO(120) })));
}

TEST(ProgramDTOTest, hasId) {
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasId());
	programDTO.setId(5);
	EXPECT_TRUE(programDTO.hasId());
}

TEST(ProgramDTOTest, hasName) {
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasName());
	programDTO.setName("abcd");
	EXPECT_TRUE(programDTO.hasName());
}

TEST(ProgramDTOTest, hasSchedulerType) {
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasSchedulerType());
	programDTO.setSchedulerType("test_scheduler");
	EXPECT_TRUE(programDTO.hasSchedulerType());
}

TEST(ProgramDTOTest, hasSpecifiedScheduler) {
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasSpecifiedScheduler());
	programDTO.setSpecifiedScheduler(SpecifiedSchedulerDTO(new list<bool>({ true, false })));
	EXPECT_TRUE(programDTO.hasSpecifiedScheduler());
}

TEST(ProgramDTOTest, hasRunTimes) {
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasRunTimes());
	programDTO.setRunTimes(new list<RunTimeDTO>({ RunTimeDTO(100), RunTimeDTO(120) }));
	EXPECT_TRUE(programDTO.hasRunTimes());
}

TEST(ProgramDTOTest, hasStartTimes) {
	ProgramDTO programDTO;

	EXPECT_FALSE(programDTO.hasStartTimes());
	programDTO.setStartTimes(new list<StartTimeDTO>({ StartTimeDTO(100), StartTimeDTO(120) }));
	EXPECT_TRUE(programDTO.hasStartTimes());
}

TEST(ProgramDTOTest, equal) {
	ProgramDTO programDTO1;
	ProgramDTO programDTO2;

	EXPECT_TRUE(programDTO1 == programDTO2);

	programDTO1.setId(10);
	EXPECT_FALSE(programDTO1 == programDTO2);

	programDTO2.setId(15);
	EXPECT_FALSE(programDTO1 == programDTO2);

	programDTO1.setId(15);
	EXPECT_TRUE(programDTO1 == programDTO2);

	programDTO1.setName("1234");
	EXPECT_FALSE(programDTO1 == programDTO2);

	programDTO2.setName("abcde");
	EXPECT_FALSE(programDTO1 == programDTO2);

	programDTO1.setName("abcde");
	EXPECT_TRUE(programDTO1 == programDTO2);

	programDTO1.setSchedulerType("scheduler_type");
	EXPECT_FALSE(programDTO1 == programDTO2);

	programDTO2.setSchedulerType("12345");
	EXPECT_FALSE(programDTO1 == programDTO2);

	programDTO1.setSchedulerType("12345");
	EXPECT_TRUE(programDTO1 == programDTO2);

	programDTO1.setSpecifiedScheduler(SpecifiedSchedulerDTO(new list<bool>({ true, true })));
	EXPECT_FALSE(programDTO1 == programDTO2);

	programDTO2.setSpecifiedScheduler(SpecifiedSchedulerDTO(new list<bool>({ true, false })));
	EXPECT_FALSE(programDTO1 == programDTO2);

	programDTO1.setSpecifiedScheduler(SpecifiedSchedulerDTO(new list<bool>({ true, false })));
	EXPECT_TRUE(programDTO1 == programDTO2);

	programDTO1.setRunTimes(new list<RunTimeDTO>({ RunTimeDTO(10), RunTimeDTO(20) }));
	EXPECT_FALSE(programDTO1 == programDTO2);

	programDTO2.setRunTimes(new list<RunTimeDTO>({ RunTimeDTO(10), RunTimeDTO(20), RunTimeDTO(30) }));
	EXPECT_FALSE(programDTO1 == programDTO2);

	programDTO1.setRunTimes(new list<RunTimeDTO>({ RunTimeDTO(10), RunTimeDTO(20), RunTimeDTO(30) }));
	EXPECT_TRUE(programDTO1 == programDTO2);

	programDTO1.setStartTimes(new list<StartTimeDTO>({ StartTimeDTO(10), StartTimeDTO(20) }));
	EXPECT_FALSE(programDTO1 == programDTO2);

	programDTO2.setStartTimes(new list<StartTimeDTO>({ StartTimeDTO(20), StartTimeDTO(10) }));
	EXPECT_FALSE(programDTO1 == programDTO2);

	programDTO1.setStartTimes(new list<StartTimeDTO>({ StartTimeDTO(20), StartTimeDTO(10) }));
	EXPECT_TRUE(programDTO1 == programDTO2);
}
