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
	DocumentDTO documentDTO(new list<ProgramDTO>({
		ProgramDTO("abc", "sch1", SpecifiedSchedulerDTO(), new list<RunTimeDTO>({ RunTimeDTO() }), new list<StartTimeDTO>()),
		ProgramDTO("123", "sch2", SpecifiedSchedulerDTO(), new list<RunTimeDTO>(), new list<StartTimeDTO>({ StartTimeDTO()}))
	}));

	EXPECT_TRUE(documentDTO.hasPrograms());
	EXPECT_THAT(documentDTO.getPrograms(),
			ContainerEq(list<ProgramDTO>({
				ProgramDTO("abc", "sch1", SpecifiedSchedulerDTO(), new list<RunTimeDTO>({ RunTimeDTO() }), new list<StartTimeDTO>()),
				ProgramDTO("123", "sch2", SpecifiedSchedulerDTO(), new list<RunTimeDTO>(), new list<StartTimeDTO>({ StartTimeDTO()}))
			})
	));
}

TEST(DocumentDTOTest, copyConstructor) {
	DocumentDTO documentDTO(
		DocumentDTO(new list<ProgramDTO>({
			ProgramDTO("abc", "sch1", SpecifiedSchedulerDTO(), new list<RunTimeDTO>({ RunTimeDTO() }), new list<StartTimeDTO>()),
			ProgramDTO("123", "sch2", SpecifiedSchedulerDTO(), new list<RunTimeDTO>(), new list<StartTimeDTO>({ StartTimeDTO()}))
		})
	));

	EXPECT_TRUE(documentDTO.hasPrograms());
	EXPECT_THAT(documentDTO.getPrograms(),
			ContainerEq(list<ProgramDTO>({
				ProgramDTO("abc", "sch1", SpecifiedSchedulerDTO(), new list<RunTimeDTO>({ RunTimeDTO() }), new list<StartTimeDTO>()),
				ProgramDTO("123", "sch2", SpecifiedSchedulerDTO(), new list<RunTimeDTO>(), new list<StartTimeDTO>({ StartTimeDTO()}))
			})
	));
}

TEST(DocumentDTOTest, getPrograms) {
	DocumentDTO documentDTO;

	EXPECT_THROW(documentDTO.getPrograms(), logic_error);
	documentDTO.setPrograms(new list<ProgramDTO>({
		ProgramDTO("abc", "sch1", SpecifiedSchedulerDTO(), new list<RunTimeDTO>({ RunTimeDTO() }), new list<StartTimeDTO>()),
		ProgramDTO("123", "sch2", SpecifiedSchedulerDTO(), new list<RunTimeDTO>(), new list<StartTimeDTO>({ StartTimeDTO()}))
	}));
	ASSERT_NO_THROW(documentDTO.getPrograms());
	EXPECT_THAT(documentDTO.getPrograms(),
			ContainerEq(list<ProgramDTO>({
				ProgramDTO("abc", "sch1", SpecifiedSchedulerDTO(), new list<RunTimeDTO>({ RunTimeDTO() }), new list<StartTimeDTO>()),
				ProgramDTO("123", "sch2", SpecifiedSchedulerDTO(), new list<RunTimeDTO>(), new list<StartTimeDTO>({ StartTimeDTO()}))
			})
	));
}

TEST(DocumentDTOTest, hasPrograms) {
	DocumentDTO documentDTO;

	EXPECT_FALSE(documentDTO.hasPrograms());
	documentDTO.setPrograms(new list<ProgramDTO>());
	EXPECT_TRUE(documentDTO.hasPrograms());
}

TEST(DocumentDTOTest, equal) {
	DocumentDTO documentDTO1;
	DocumentDTO documentDTO2;

	EXPECT_TRUE(documentDTO1 == documentDTO2);

	documentDTO1.setPrograms(new list<ProgramDTO>({
		ProgramDTO("abc", "sch1", SpecifiedSchedulerDTO(), new list<RunTimeDTO>({ RunTimeDTO() }), new list<StartTimeDTO>())
	}));
	EXPECT_FALSE(documentDTO1 == documentDTO2);

	documentDTO2.setPrograms(new list<ProgramDTO>({
		ProgramDTO("123", "sch2", SpecifiedSchedulerDTO(), new list<RunTimeDTO>(), new list<StartTimeDTO>({ StartTimeDTO()}))
	}));
	EXPECT_FALSE(documentDTO1 == documentDTO2);

	documentDTO1.setPrograms(new list<ProgramDTO>({
		ProgramDTO("123", "sch2", SpecifiedSchedulerDTO(), new list<RunTimeDTO>(), new list<StartTimeDTO>({ StartTimeDTO()}))
	}));
	EXPECT_TRUE(documentDTO1 == documentDTO2);
}

