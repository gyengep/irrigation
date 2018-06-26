#include <gmock/gmock.h>
#include <stdexcept>
#include "DTO/DocumentDTO.h"

using namespace std;
using namespace testing;



TEST(DocumentDTOTest, defaultConstructor) {
	DocumentDTO documentDTO;

	EXPECT_FALSE(documentDTO.hasPrograms());
}

TEST(DocumentDTOTest, constructor) {
	const list<ProgramDTO> expectedPrograms({
		ProgramDTO("abc", "sch1", SpecifiedSchedulerDTO(), new list<RunTimeDTO>({ RunTimeDTO() }), new list<StartTimeDTO>()),
		ProgramDTO("123", "sch2", SpecifiedSchedulerDTO(), new list<RunTimeDTO>(), new list<StartTimeDTO>({ StartTimeDTO()}))
	});

	DocumentDTO documentDTO(new list<ProgramDTO>(expectedPrograms));

	EXPECT_TRUE(documentDTO.hasPrograms());
	EXPECT_THAT(documentDTO.getPrograms(),	ContainerEq(expectedPrograms));
}

TEST(DocumentDTOTest, copyConstructor) {
	const list<ProgramDTO> expectedPrograms({
		ProgramDTO("abc", "sch1", SpecifiedSchedulerDTO(), new list<RunTimeDTO>({ RunTimeDTO() }), new list<StartTimeDTO>()),
		ProgramDTO("123", "sch2", SpecifiedSchedulerDTO(), new list<RunTimeDTO>(), new list<StartTimeDTO>({ StartTimeDTO()}))
	});

	const DocumentDTO other(new list<ProgramDTO>(expectedPrograms));

	DocumentDTO documentDTO(other);

	EXPECT_TRUE(documentDTO.hasPrograms());
	EXPECT_THAT(documentDTO.getPrograms(),	ContainerEq(expectedPrograms));
}

TEST(DocumentDTOTest, hasPrograms) {
	DocumentDTO documentDTO;

	EXPECT_FALSE(documentDTO.hasPrograms());
	documentDTO.setPrograms(new list<ProgramDTO>());
	EXPECT_TRUE(documentDTO.hasPrograms());
}

TEST(DocumentDTOTest, getPrograms) {
	const list<ProgramDTO> expectedPrograms({
		ProgramDTO("abc", "sch1", SpecifiedSchedulerDTO(), new list<RunTimeDTO>({ RunTimeDTO() }), new list<StartTimeDTO>()),
		ProgramDTO("123", "sch2", SpecifiedSchedulerDTO(), new list<RunTimeDTO>(), new list<StartTimeDTO>({ StartTimeDTO()}))
	});

	DocumentDTO documentDTO;

	EXPECT_THROW(documentDTO.getPrograms(), logic_error);
	documentDTO.setPrograms(new list<ProgramDTO>(expectedPrograms));
	ASSERT_NO_THROW(documentDTO.getPrograms());
	EXPECT_THAT(documentDTO.getPrograms(),	ContainerEq(expectedPrograms));
}

TEST(DocumentDTOTest, equal) {
	const list<ProgramDTO> expectedPrograms1({
		ProgramDTO("abc", "sch1", SpecifiedSchedulerDTO(), new list<RunTimeDTO>({ RunTimeDTO() }), new list<StartTimeDTO>())
	});

	const list<ProgramDTO> expectedPrograms2({
		ProgramDTO("123", "sch2", SpecifiedSchedulerDTO(), new list<RunTimeDTO>(), new list<StartTimeDTO>({ StartTimeDTO()}))
	});

	DocumentDTO documentDTO1;
	DocumentDTO documentDTO2;

	EXPECT_TRUE(documentDTO1 == documentDTO2);
	EXPECT_TRUE(documentDTO2 == documentDTO1);

	documentDTO1.setPrograms(new list<ProgramDTO>(expectedPrograms1));
	EXPECT_FALSE(documentDTO1 == documentDTO2);
	EXPECT_FALSE(documentDTO2 == documentDTO1);

	documentDTO2.setPrograms(new list<ProgramDTO>(expectedPrograms2));
	EXPECT_FALSE(documentDTO1 == documentDTO2);
	EXPECT_FALSE(documentDTO2 == documentDTO1);

	documentDTO1.setPrograms(new list<ProgramDTO>(expectedPrograms2));
	EXPECT_TRUE(documentDTO1 == documentDTO2);
	EXPECT_TRUE(documentDTO2 == documentDTO1);
}

