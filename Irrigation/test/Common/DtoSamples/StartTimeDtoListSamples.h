#pragma once
#include "DTO/StartTimeDto.h"


namespace DtoSamples {

	const StartTimeDtoList startTimeDtoListSample1(
			std::list<StartTimeDto> {
				StartTimeDto(7, 15),
				StartTimeDto(8, 20)
			}
		);

	const StartTimeDtoList startTimeDtoListSample2(
			std::list<StartTimeDto> {
				StartTimeDto(2, 11),
				StartTimeDto(3, 12),
				StartTimeDto(4, 13),
			}
		);

	const StartTimeDtoList startTimeDtoListSample3(
			std::list<StartTimeDto> {
				StartTimeDto(16, 55),
				StartTimeDto(17, 56),
				StartTimeDto(18, 57),
				StartTimeDto(19, 58)
			}
		);

}
