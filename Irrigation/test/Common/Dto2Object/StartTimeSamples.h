#pragma once
#include <memory>
#include <vector>
#include "DTO/StartTimeDTO.h"
#include "Logic/StartTimeImpl.h"
#include "SampleBase.h"


namespace Dto2ObjectTest {

	typedef ObjectSample<StartTimeImpl, StartTimeDTO> StartTimeSample;

	class StartTimeSampleList : public std::vector<StartTimeSample> {
	public:
		StartTimeSampleList() {

			emplace_back(
				std::make_shared<StartTimeImpl>(10, 20),
				StartTimeDTO(10, 20)
			);

			emplace_back(
				std::make_shared<StartTimeImpl>(11, 25),
				StartTimeDTO(11, 25)
			);

			emplace_back(
				std::make_shared<StartTimeImpl>(12, 30),
				StartTimeDTO(12, 30)
			);

			emplace_back(
				std::make_shared<StartTimeImpl>(13, 35),
				StartTimeDTO(13, 35)
			);
		}
	};
};
