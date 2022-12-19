#pragma once
#include <memory>
#include <vector>
#include "DTO/StartTimeDTO.h"
#include "SampleBase.h"


namespace Dto2XmlTest {

	typedef Sample<StartTimeDTO> StartTimeSample;

	class StartTimeSampleList : public std::vector<StartTimeSample> {
	public:
		StartTimeSampleList() {

			emplace_back(
				StartTimeDTO(10, 20),
				"<starttime%s><hour>10</hour><minute>20</minute></starttime>"
			);

			emplace_back(
				StartTimeDTO(11, 25),
				"<starttime%s><hour>11</hour><minute>25</minute></starttime>"
			);

			emplace_back(
				StartTimeDTO(12, 30),
				"<starttime%s><hour>12</hour><minute>30</minute></starttime>"
			);

			emplace_back(
				StartTimeDTO(13, 35),
				"<starttime%s><hour>13</hour><minute>35</minute></starttime>"
			);
		}
	};
};
