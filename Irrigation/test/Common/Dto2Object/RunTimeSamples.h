#pragma once
#include <memory>
#include <vector>
#include "DTO/RunTimeDTO.h"
#include "Logic/RunTimeImpl.h"
#include "SampleBase.h"


namespace Dto2ObjectTest {

	typedef ObjectSample<RunTimeImpl, RunTimeDTO> RunTimeSample;

	class RunTimeSampleList : public std::vector<RunTimeSample> {
	public:
		RunTimeSampleList() {

			emplace_back(
				std::make_shared<RunTimeImpl>(std::chrono::minutes(1) + std::chrono::seconds(10)),
				RunTimeDTO(1, 10)
			);

			emplace_back(
				std::make_shared<RunTimeImpl>(std::chrono::minutes(2) + std::chrono::seconds(15)),
				RunTimeDTO(2, 15)
			);

			emplace_back(
				std::make_shared<RunTimeImpl>(std::chrono::minutes(3) + std::chrono::seconds(20)),
				RunTimeDTO(3, 20)
			);

			emplace_back(
				std::make_shared<RunTimeImpl>(std::chrono::minutes(4) + std::chrono::seconds(30)),
				RunTimeDTO(4, 30)
			);

			emplace_back(
				std::make_shared<RunTimeImpl>(std::chrono::minutes(5) + std::chrono::seconds(40)),
				RunTimeDTO(5, 40)
			);

			emplace_back(
				std::make_shared<RunTimeImpl>(std::chrono::minutes(10) + std::chrono::seconds(50)),
				RunTimeDTO(10, 50)
			);
		}
	};
};
