#include <gmock/gmock.h>
#include <stdexcept>
#include "DTO/DocumentDto.h"
#include "DtoTestMacros.h"

using namespace std;
using namespace testing;


TEST(DocumentDtoTest, defaultConstructor) {
	DocumentDto documentDto;

	EXPECT_FALSE(documentDto.hasPrograms());
	EXPECT_THROW(documentDto.getPrograms(), logic_error);
}

TEST(DocumentDtoTest, parametrizedConstructor) {
	const ProgramDtoList expectedProgramDtoList({
		ProgramDto(true, "abc", 83, "sch1",
				SchedulersDto(),
				RunTimeDtoList({ RunTimeDto() }),
				StartTimeDtoList()
			),
		ProgramDto(false, "123", 125, "sch2",
				SchedulersDto(),
				RunTimeDtoList(),
				StartTimeDtoList({ StartTimeDto()})
			)
	});

	DocumentDto documentDto(move(ProgramDtoList(expectedProgramDtoList)));

	EXPECT_TRUE(documentDto.hasPrograms());
	EXPECT_THAT(documentDto.getPrograms(),	ContainerEq(expectedProgramDtoList));
}

TEST(DocumentDtoTest, copyConstructor) {
	const ProgramDtoList expectedProgramDtoList({
		ProgramDto(true, "abc", 83, "sch1",
				SchedulersDto(),
				RunTimeDtoList({ RunTimeDto() }),
				StartTimeDtoList()
				),
		ProgramDto(false, "123", 125, "sch2",
				SchedulersDto(),
				RunTimeDtoList(),
				StartTimeDtoList({ StartTimeDto()})
				)
	});

	const DocumentDto source(move(ProgramDtoList(expectedProgramDtoList)));
	DocumentDto documentDto(source);

	EXPECT_TRUE(documentDto.hasPrograms());
	EXPECT_THAT(documentDto.getPrograms(),	ContainerEq(expectedProgramDtoList));
}

TEST(DocumentDtoTest, moveConstructor) {
	const ProgramDtoList expectedProgramDtoList({
		ProgramDto(true, "abc", 83, "sch1",
				SchedulersDto(),
				RunTimeDtoList({ RunTimeDto() }),
				StartTimeDtoList()
				),
		ProgramDto(false, "123", 125, "sch2",
				SchedulersDto(),
				RunTimeDtoList(),
				StartTimeDtoList({ StartTimeDto()})
				)
	});

	DocumentDto source(move(ProgramDtoList(expectedProgramDtoList)));
	DocumentDto documentDto(move(source));

	EXPECT_TRUE(documentDto.hasPrograms());
	EXPECT_THAT(documentDto.getPrograms(),	ContainerEq(expectedProgramDtoList));
}

///////////////////////////////////////////////////////////////////////////////

CHECK_DTO_FUNCTIONS(DocumentDto, ProgramDtoList, Programs,
	ProgramDtoList({
		ProgramDto(true, "abc", 83, "sch1",
				SchedulersDto(),
				RunTimeDtoList({ RunTimeDto() }),
				StartTimeDtoList()
				),
		ProgramDto(false, "123", 125, "sch2",
				SchedulersDto(),
				RunTimeDtoList(),
				StartTimeDtoList({ StartTimeDto()})
				)
	})
);

///////////////////////////////////////////////////////////////////////////////

TEST(DocumentDtoTest, equalsOperator) {
	DocumentDto dto1;
	DocumentDto dto2;

	EXPECT_TRUE(dto1 == dto2);
	EXPECT_TRUE(dto2 == dto1);

	CHECK_DTO_EQUALS_MOVE(ProgramDtoList, Programs,
			ProgramDtoList({
				ProgramDto(true, "abc", 83, "sch1",
						SchedulersDto(),
						RunTimeDtoList({ RunTimeDto() }),
						StartTimeDtoList()
					)
			}),
			ProgramDtoList({
				ProgramDto(false, "123", 125, "sch2",
						SchedulersDto(),
						RunTimeDtoList(),
						StartTimeDtoList({ StartTimeDto()})
					)
			})
		);
}
