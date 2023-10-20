#pragma once
#include <memory>
#include <vector>
#include "DTO/RunTimeDto.h"
#include "Logic/Impl/RunTimeImpl.h"
#include "SampleBase.h"


namespace Dto2ObjectTestSamples {

	typedef ObjectSample<RunTimeImpl, RunTimeDto> RunTimeSample;

	class RunTimeSampleList : public std::vector<RunTimeSample> {
	public:
		RunTimeSampleList() {

			emplace_back(
				std::make_shared<RunTimeImpl>(std::chrono::minutes(1) + std::chrono::seconds(10)),
				RunTimeDto(1, 10)
			);

			emplace_back(
				std::make_shared<RunTimeImpl>(std::chrono::minutes(2) + std::chrono::seconds(15)),
				RunTimeDto(2, 15)
			);

			emplace_back(
				std::make_shared<RunTimeImpl>(std::chrono::minutes(3) + std::chrono::seconds(20)),
				RunTimeDto(3, 20)
			);

			emplace_back(
				std::make_shared<RunTimeImpl>(std::chrono::minutes(4) + std::chrono::seconds(30)),
				RunTimeDto(4, 30)
			);

			emplace_back(
				std::make_shared<RunTimeImpl>(std::chrono::minutes(5) + std::chrono::seconds(40)),
				RunTimeDto(5, 40)
			);

			emplace_back(
				std::make_shared<RunTimeImpl>(std::chrono::minutes(10) + std::chrono::seconds(50)),
				RunTimeDto(10, 50)
			);
		}
	};
};
