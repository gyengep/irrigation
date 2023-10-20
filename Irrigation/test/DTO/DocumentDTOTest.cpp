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
	const list<ProgramDto> expectedPrograms({
		ProgramDto(true, "abc", 83, "sch1",
				SchedulersDto(),
				list<RunTimeDto>({ RunTimeDto() }),
				list<StartTimeDto>()
			),
		ProgramDto(false, "123", 125, "sch2",
				SchedulersDto(),
				list<RunTimeDto>(),
				list<StartTimeDto>({ StartTimeDto()})
			)
	});

	DocumentDto documentDto(move(list<ProgramDto>(expectedPrograms)));

	EXPECT_TRUE(documentDto.hasPrograms());
	EXPECT_THAT(documentDto.getPrograms(),	ContainerEq(expectedPrograms));
}

TEST(DocumentDtoTest, copyConstructor) {
	const list<ProgramDto> expectedPrograms({
		ProgramDto(true, "abc", 83, "sch1",
				SchedulersDto(),
				list<RunTimeDto>({ RunTimeDto() }),
				list<StartTimeDto>()
				),
		ProgramDto(false, "123", 125, "sch2",
				SchedulersDto(),
				list<RunTimeDto>(),
				list<StartTimeDto>({ StartTimeDto()})
				)
	});

	const DocumentDto source(move(list<ProgramDto>(expectedPrograms)));
	DocumentDto documentDto(source);

	EXPECT_TRUE(documentDto.hasPrograms());
	EXPECT_THAT(documentDto.getPrograms(),	ContainerEq(expectedPrograms));
}

TEST(DocumentDtoTest, moveConstructor) {
	const list<ProgramDto> expectedPrograms({
		ProgramDto(true, "abc", 83, "sch1",
				SchedulersDto(),
				list<RunTimeDto>({ RunTimeDto() }),
				list<StartTimeDto>()
				),
		ProgramDto(false, "123", 125, "sch2",
				SchedulersDto(),
				list<RunTimeDto>(),
				list<StartTimeDto>({ StartTimeDto()})
				)
	});

	DocumentDto source(move(list<ProgramDto>(expectedPrograms)));
	DocumentDto documentDto(move(source));

	EXPECT_TRUE(documentDto.hasPrograms());
	EXPECT_THAT(documentDto.getPrograms(),	ContainerEq(expectedPrograms));
}

///////////////////////////////////////////////////////////////////////////////

CHECK_DTO_FUNCTIONS(DocumentDto, list<ProgramDto>, Programs,
	list<ProgramDto>({
		ProgramDto(true, "abc", 83, "sch1",
				SchedulersDto(),
				list<RunTimeDto>({ RunTimeDto() }),
				list<StartTimeDto>()
				),
		ProgramDto(false, "123", 125, "sch2",
				SchedulersDto(),
				list<RunTimeDto>(),
				list<StartTimeDto>({ StartTimeDto()})
				)
	})
);

///////////////////////////////////////////////////////////////////////////////

TEST(DocumentDtoTest, equalsOperator) {
	DocumentDto dto1;
	DocumentDto dto2;

	EXPECT_TRUE(dto1 == dto2);
	EXPECT_TRUE(dto2 == dto1);

	CHECK_DTO_EQUALS_MOVE(list<ProgramDto>, Programs,
			list<ProgramDto>({
				ProgramDto(true, "abc", 83, "sch1",
						SchedulersDto(),
						list<RunTimeDto>({ RunTimeDto() }),
						list<StartTimeDto>()
					)
			}),
			list<ProgramDto>({
				ProgramDto(false, "123", 125, "sch2",
						SchedulersDto(),
						list<RunTimeDto>(),
						list<StartTimeDto>({ StartTimeDto()})
					)
			})
		);
}
