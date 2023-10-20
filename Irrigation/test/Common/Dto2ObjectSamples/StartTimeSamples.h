#pragma once
#include <memory>
#include <vector>
#include "DTO/StartTimeDto.h"
#include "Logic/Impl/StartTimeImpl.h"
#include "SampleBase.h"


namespace Dto2ObjectTestSamples {

	typedef ObjectSample<StartTimeImpl, StartTimeDto> StartTimeSample;

	class StartTimeSampleList : public std::vector<StartTimeSample> {
	public:
		StartTimeSampleList() {

			emplace_back(
				std::make_shared<StartTimeImpl>(10, 20),
				StartTimeDto(10, 20)
			);

			emplace_back(
				std::make_shared<StartTimeImpl>(11, 25),
				StartTimeDto(11, 25)
			);

			emplace_back(
				std::make_shared<StartTimeImpl>(12, 30),
				StartTimeDto(12, 30)
			);

			emplace_back(
				std::make_shared<StartTimeImpl>(13, 35),
				StartTimeDto(13, 35)
			);
		}
	};
};
