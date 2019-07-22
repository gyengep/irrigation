#include <gmock/gmock.h>
#include <stdexcept>
#include "DTO/DocumentDTO.h"

using namespace std;
using namespace testing;



TEST(DocumentDTOTest, defaultConstructor) {
	DocumentDTO documentDTO;

	EXPECT_FALSE(documentDTO.hasPrograms());
}

TEST(DocumentDTOTest, parametrizedConstructor) {
	const list<ProgramDTO> expectedPrograms({
		ProgramDTO(true, "abc", 83, "sch1", PeriodicSchedulerDTO(), WeeklySchedulerDTO(), EveryDaySchedulerDTO(), list<RunTimeDTO>({ RunTimeDTO() }), list<StartTimeDTO>()),
		ProgramDTO(false, "123", 125, "sch2", PeriodicSchedulerDTO(), WeeklySchedulerDTO(), EveryDaySchedulerDTO(), list<RunTimeDTO>(), list<StartTimeDTO>({ StartTimeDTO()}))
	});

	DocumentDTO documentDTO(move(list<ProgramDTO>(expectedPrograms)));

	EXPECT_TRUE(documentDTO.hasPrograms());
	EXPECT_THAT(documentDTO.getPrograms(),	ContainerEq(expectedPrograms));
}

TEST(DocumentDTOTest, copyConstructor) {
	const list<ProgramDTO> expectedPrograms({
		ProgramDTO(true, "abc", 83, "sch1", PeriodicSchedulerDTO(), WeeklySchedulerDTO(), EveryDaySchedulerDTO(), list<RunTimeDTO>({ RunTimeDTO() }), list<StartTimeDTO>()),
		ProgramDTO(false, "123", 125, "sch2", PeriodicSchedulerDTO(), WeeklySchedulerDTO(), EveryDaySchedulerDTO(), list<RunTimeDTO>(), list<StartTimeDTO>({ StartTimeDTO()}))
	});

	const DocumentDTO source(move(list<ProgramDTO>(expectedPrograms)));
	DocumentDTO documentDTO(source);

	EXPECT_TRUE(documentDTO.hasPrograms());
	EXPECT_THAT(documentDTO.getPrograms(),	ContainerEq(expectedPrograms));
}

TEST(DocumentDTOTest, moveConstructor) {
	const list<ProgramDTO> expectedPrograms({
		ProgramDTO(true, "abc", 83, "sch1", PeriodicSchedulerDTO(), WeeklySchedulerDTO(), EveryDaySchedulerDTO(), list<RunTimeDTO>({ RunTimeDTO() }), list<StartTimeDTO>()),
		ProgramDTO(false, "123", 125, "sch2", PeriodicSchedulerDTO(), WeeklySchedulerDTO(), EveryDaySchedulerDTO(), list<RunTimeDTO>(), list<StartTimeDTO>({ StartTimeDTO()}))
	});

	DocumentDTO source(move(list<ProgramDTO>(expectedPrograms)));
	DocumentDTO documentDTO(move(source));

	EXPECT_TRUE(documentDTO.hasPrograms());
	EXPECT_THAT(documentDTO.getPrograms(),	ContainerEq(expectedPrograms));
}

TEST(DocumentDTOTest, hasPrograms) {
	DocumentDTO documentDTO;

	EXPECT_FALSE(documentDTO.hasPrograms());
	documentDTO.setPrograms(list<ProgramDTO>());
	EXPECT_TRUE(documentDTO.hasPrograms());
}

TEST(DocumentDTOTest, getPrograms) {
	const list<ProgramDTO> expectedPrograms({
		ProgramDTO(true, "abc", 83, "sch1", PeriodicSchedulerDTO(), WeeklySchedulerDTO(), EveryDaySchedulerDTO(), list<RunTimeDTO>({ RunTimeDTO() }), list<StartTimeDTO>()),
		ProgramDTO(false, "123", 125, "sch2", PeriodicSchedulerDTO(), WeeklySchedulerDTO(), EveryDaySchedulerDTO(), list<RunTimeDTO>(), list<StartTimeDTO>({ StartTimeDTO()}))
	});

	DocumentDTO documentDTO;

	EXPECT_THROW(documentDTO.getPrograms(), logic_error);
	documentDTO.setPrograms(list<ProgramDTO>(expectedPrograms));
	ASSERT_NO_THROW(documentDTO.getPrograms());
	EXPECT_THAT(documentDTO.getPrograms(),	ContainerEq(expectedPrograms));
}

TEST(DocumentDTOTest, equalsOperator) {
	const list<ProgramDTO> expectedPrograms1({
		ProgramDTO(true, "abc", 83, "sch1", PeriodicSchedulerDTO(), WeeklySchedulerDTO(), EveryDaySchedulerDTO(), list<RunTimeDTO>({ RunTimeDTO() }), list<StartTimeDTO>())
	});

	const list<ProgramDTO> expectedPrograms2({
		ProgramDTO(false, "123", 125, "sch2", PeriodicSchedulerDTO(), WeeklySchedulerDTO(), EveryDaySchedulerDTO(), list<RunTimeDTO>(), list<StartTimeDTO>({ StartTimeDTO()}))
	});

	DocumentDTO documentDTO1;
	DocumentDTO documentDTO2;

	EXPECT_TRUE(documentDTO1 == documentDTO2);
	EXPECT_TRUE(documentDTO2 == documentDTO1);

	documentDTO1.setPrograms(list<ProgramDTO>(expectedPrograms1));
	EXPECT_FALSE(documentDTO1 == documentDTO2);
	EXPECT_FALSE(documentDTO2 == documentDTO1);

	documentDTO2.setPrograms(list<ProgramDTO>(expectedPrograms2));
	EXPECT_FALSE(documentDTO1 == documentDTO2);
	EXPECT_FALSE(documentDTO2 == documentDTO1);

	documentDTO1.setPrograms(list<ProgramDTO>(expectedPrograms2));
	EXPECT_TRUE(documentDTO1 == documentDTO2);
	EXPECT_TRUE(documentDTO2 == documentDTO1);
}

