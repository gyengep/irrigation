#pragma once
#include <memory>
#include <vector>
#include "DTO/StartTimeDto.h"
#include "SampleBase.h"


namespace RestViewTestSamples {

	typedef Sample<StartTimeDto> StartTimeSample;

	class StartTimeSampleList : public std::vector<StartTimeSample> {
	public:
		StartTimeSampleList() {

			emplace_back(
				StartTimeDto(10, 20),
				"<starttime%s><hour>10</hour><minute>20</minute></starttime>"
			);

			emplace_back(
				StartTimeDto(11, 25),
				"<starttime%s><hour>11</hour><minute>25</minute></starttime>"
			);

			emplace_back(
				StartTimeDto(12, 30),
				"<starttime%s><hour>12</hour><minute>30</minute></starttime>"
			);

			emplace_back(
				StartTimeDto(13, 35),
				"<starttime%s><hour>13</hour><minute>35</minute></starttime>"
			);
		}
	};
};
