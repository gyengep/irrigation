#pragma once
#include <memory>
#include <vector>
#include "DTO/WeeklySchedulerDto.h"
#include "SampleBase.h"


namespace RestViewTestSamples {

	typedef Sample<WeeklySchedulerDto> WeeklySchedulerSample;

	class WeeklySchedulerSampleList : public std::vector<WeeklySchedulerSample> {
	public:
		WeeklySchedulerSampleList() {

			emplace_back(
				WeeklySchedulerDto(std::list<bool> { true, false, true, false, true, false, true }),
				"<scheduler type=\"weekly\">"
					"<days>"
						"<day>true</day>"
						"<day>false</day>"
						"<day>true</day>"
						"<day>false</day>"
						"<day>true</day>"
						"<day>false</day>"
						"<day>true</day>"
					"</days>"
				"</scheduler>"
			);

			emplace_back(
				WeeklySchedulerDto(std::list<bool> { false, true, false, true, false, true, false }),
				"<scheduler type=\"weekly\">"
					"<days>"
						"<day>false</day>"
						"<day>true</day>"
						"<day>false</day>"
						"<day>true</day>"
						"<day>false</day>"
						"<day>true</day>"
						"<day>false</day>"
					"</days>"
				"</scheduler>"
			);

			emplace_back(
				WeeklySchedulerDto(std::list<bool> { true, true, false, false, true, false, true }),
				"<scheduler type=\"weekly\">"
					"<days>"
						"<day>true</day>"
						"<day>true</day>"
						"<day>false</day>"
						"<day>false</day>"
						"<day>true</day>"
						"<day>false</day>"
						"<day>true</day>"
					"</days>"
				"</scheduler>"
			);

			emplace_back(
				WeeklySchedulerDto(std::list<bool> { false, false, false, false, true, false, true }),
				"<scheduler type=\"weekly\">"
					"<days>"
						"<day>false</day>"
						"<day>false</day>"
						"<day>false</day>"
						"<day>false</day>"
						"<day>true</day>"
						"<day>true</day>"
						"<day>true</day>"
					"</days>"
				"</scheduler>"
			);
		}
	};
};
