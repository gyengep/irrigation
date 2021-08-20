#include <gmock/gmock.h>
#include <stdexcept>
#include "DTO/DocumentDTO.h"
#include "DtoTestMacros.h"

using namespace std;
using namespace testing;


TEST(DocumentDTOTest, defaultConstructor) {
	DocumentDTO documentDTO;

	EXPECT_FALSE(documentDTO.hasPrograms());
	EXPECT_THROW(documentDTO.getPrograms(), logic_error);
}

TEST(DocumentDTOTest, parametrizedConstructor) {
	const list<ProgramDTO> expectedPrograms({
		ProgramDTO(true, "abc", 83, "sch1",
				EveryDaySchedulerDTO(),
				HotWeatherSchedulerDTO(),
				TemperatureDependentSchedulerDTO(),
				WeeklySchedulerDTO(),
				list<RunTimeDTO>({ RunTimeDTO() }),
				list<StartTimeDTO>()
				),
		ProgramDTO(false, "123", 125, "sch2",
				EveryDaySchedulerDTO(),
				HotWeatherSchedulerDTO(),
				TemperatureDependentSchedulerDTO(),
				WeeklySchedulerDTO(),
				list<RunTimeDTO>(),
				list<StartTimeDTO>({ StartTimeDTO()})
				)
	});

	DocumentDTO documentDTO(move(list<ProgramDTO>(expectedPrograms)));

	EXPECT_TRUE(documentDTO.hasPrograms());
	EXPECT_THAT(documentDTO.getPrograms(),	ContainerEq(expectedPrograms));
}

TEST(DocumentDTOTest, copyConstructor) {
	const list<ProgramDTO> expectedPrograms({
		ProgramDTO(true, "abc", 83, "sch1",
				EveryDaySchedulerDTO(),
				HotWeatherSchedulerDTO(),
				TemperatureDependentSchedulerDTO(),
				WeeklySchedulerDTO(),
				list<RunTimeDTO>({ RunTimeDTO() }),
				list<StartTimeDTO>()
				),
		ProgramDTO(false, "123", 125, "sch2",
				EveryDaySchedulerDTO(),
				HotWeatherSchedulerDTO(),
				TemperatureDependentSchedulerDTO(),
				WeeklySchedulerDTO(),
				list<RunTimeDTO>(),
				list<StartTimeDTO>({ StartTimeDTO()})
				)
	});

	const DocumentDTO source(move(list<ProgramDTO>(expectedPrograms)));
	DocumentDTO documentDTO(source);

	EXPECT_TRUE(documentDTO.hasPrograms());
	EXPECT_THAT(documentDTO.getPrograms(),	ContainerEq(expectedPrograms));
}

TEST(DocumentDTOTest, moveConstructor) {
	const list<ProgramDTO> expectedPrograms({
		ProgramDTO(true, "abc", 83, "sch1",
				EveryDaySchedulerDTO(),
				HotWeatherSchedulerDTO(),
				TemperatureDependentSchedulerDTO(),
				WeeklySchedulerDTO(),
				list<RunTimeDTO>({ RunTimeDTO() }),
				list<StartTimeDTO>()
				),
		ProgramDTO(false, "123", 125, "sch2",
				EveryDaySchedulerDTO(),
				HotWeatherSchedulerDTO(),
				TemperatureDependentSchedulerDTO(),
				WeeklySchedulerDTO(),
				list<RunTimeDTO>(),
				list<StartTimeDTO>({ StartTimeDTO()})
				)
	});

	DocumentDTO source(move(list<ProgramDTO>(expectedPrograms)));
	DocumentDTO documentDTO(move(source));

	EXPECT_TRUE(documentDTO.hasPrograms());
	EXPECT_THAT(documentDTO.getPrograms(),	ContainerEq(expectedPrograms));
}

///////////////////////////////////////////////////////////////////////////////

CHECK_DTO_FUNCTIONS(DocumentDTO, list<ProgramDTO>, Programs,
	list<ProgramDTO>({
		ProgramDTO(true, "abc", 83, "sch1",
				EveryDaySchedulerDTO(),
				HotWeatherSchedulerDTO(),
				TemperatureDependentSchedulerDTO(),
				WeeklySchedulerDTO(),
				list<RunTimeDTO>({ RunTimeDTO() }),
				list<StartTimeDTO>()
				),
		ProgramDTO(false, "123", 125, "sch2",
				EveryDaySchedulerDTO(),
				HotWeatherSchedulerDTO(),
				TemperatureDependentSchedulerDTO(),
				WeeklySchedulerDTO(),
				list<RunTimeDTO>(),
				list<StartTimeDTO>({ StartTimeDTO()})
				)
	})
);

///////////////////////////////////////////////////////////////////////////////

TEST(DocumentDTOTest, equalsOperator) {
	DocumentDTO dto1;
	DocumentDTO dto2;

	EXPECT_TRUE(dto1 == dto2);
	EXPECT_TRUE(dto2 == dto1);

	CHECK_DTO_EQUALS_MOVE(list<ProgramDTO>, Programs,
			list<ProgramDTO>({
				ProgramDTO(true, "abc", 83, "sch1",
						EveryDaySchedulerDTO(),
						HotWeatherSchedulerDTO(),
						TemperatureDependentSchedulerDTO(),
						WeeklySchedulerDTO(),
						list<RunTimeDTO>({ RunTimeDTO() }),
						list<StartTimeDTO>()
					)
			}),
			list<ProgramDTO>({
				ProgramDTO(false, "123", 125, "sch2",
						EveryDaySchedulerDTO(),
						HotWeatherSchedulerDTO(),
						TemperatureDependentSchedulerDTO(),
						WeeklySchedulerDTO(),
						list<RunTimeDTO>(),
						list<StartTimeDTO>({ StartTimeDTO()})
					)
			})
		);
}
