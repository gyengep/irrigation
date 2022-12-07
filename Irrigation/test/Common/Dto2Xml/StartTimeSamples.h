#pragma once
#include <string>
#include <vector>
#include "DTO/StartTimeDTO.h"
#include "SampleBase.h"


namespace Dto2XmlTest {

	typedef Sample<StartTimeDTO> StartTimeSample;

	class StartTimeSampleList : public std::vector<StartTimeSample> {
	public:
		StartTimeSampleList() {

			emplace_back(
				StartTimeDTO(),
				"<starttime/>"
			);

			emplace_back(
				StartTimeDTO().setId(45152),
				"<starttime id=\"45152\"/>"
			);

			emplace_back(
				StartTimeDTO().setHours(11),
				"<starttime><hour>11</hour></starttime>"
			);

			emplace_back(
				StartTimeDTO().setMinutes(12),
				"<starttime><minute>12</minute></starttime>"
			);

			emplace_back(
				StartTimeDTO(1, 2).setId(8465),
				"<starttime id=\"8465\"><hour>1</hour><minute>2</minute></starttime>"
			);
		}
	};
};
