#pragma once
#include "DTO/RunTimeDto.h"


namespace DtoSamples {

	const RunTimeDtoList runTimeDtoListSample1(
			std::list<RunTimeDto> {
				RunTimeDto(10, 20),
				RunTimeDto(11, 21),
				RunTimeDto(12, 22),
				RunTimeDto(13, 23),
				RunTimeDto(14, 24),
				RunTimeDto(15, 25)
			}
		);

	const RunTimeDtoList runTimeDtoListSample2(
			std::list<RunTimeDto> {
				RunTimeDto(9, 10),
				RunTimeDto(8, 11),
				RunTimeDto(7, 12),
				RunTimeDto(6, 13),
				RunTimeDto(5, 14),
				RunTimeDto(4, 15)
			}
		);

	const RunTimeDtoList runTimeDtoListSample3(
			std::list<RunTimeDto> {
				RunTimeDto(19, 30),
				RunTimeDto(18, 31),
				RunTimeDto(17, 32),
				RunTimeDto(16, 33),
				RunTimeDto(15, 34),
				RunTimeDto(14, 35)
			}
		);

}
