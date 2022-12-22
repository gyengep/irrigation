#pragma once
#include <memory>
#include <vector>
#include "DTO/RunTimeDTO.h"
#include "SampleBase.h"


namespace RestViewTestSamples {

	typedef Sample<RunTimeDTO> RunTimeSample;

	class RunTimeSampleList : public std::vector<RunTimeSample> {
	public:
		RunTimeSampleList() {

			emplace_back(
				RunTimeDTO(1, 10),
				"<runtime%s><minute>1</minute><second>10</second></runtime>"
			);

			emplace_back(
				RunTimeDTO(2, 15),
				"<runtime%s><minute>2</minute><second>15</second></runtime>"
			);

			emplace_back(
				RunTimeDTO(3, 20),
				"<runtime%s><minute>3</minute><second>20</second></runtime>"
			);

			emplace_back(
				RunTimeDTO(4, 30),
				"<runtime%s><minute>4</minute><second>30</second></runtime>"
			);

			emplace_back(
				RunTimeDTO(5, 40),
				"<runtime%s><minute>5</minute><second>40</second></runtime>"
			);

			emplace_back(
				RunTimeDTO(10, 50),
				"<runtime%s><minute>10</minute><second>50</second></runtime>"
			);
		}
	};
};
