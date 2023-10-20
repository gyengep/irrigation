#pragma once
#include <string>
#include <vector>
#include "DTO/StartTimeDto.h"
#include "SampleBase.h"


namespace DtoReaderWriterTestSamples {

	typedef Sample<StartTimeDto> StartTimeSample;

	class StartTimeSampleList : public std::vector<StartTimeSample> {
	public:
		StartTimeSampleList() {

			emplace_back(
				StartTimeDto(),
				"<starttime/>"
			);

			emplace_back(
				StartTimeDto().setId(45152),
				"<starttime id=\"45152\"/>"
			);

			emplace_back(
				StartTimeDto().setHours(11),
				"<starttime><hour>11</hour></starttime>"
			);

			emplace_back(
				StartTimeDto().setMinutes(12),
				"<starttime><minute>12</minute></starttime>"
			);

			emplace_back(
				StartTimeDto(1, 2).setId(8465),
				"<starttime id=\"8465\"><hour>1</hour><minute>2</minute></starttime>"
			);
		}
	};
};
