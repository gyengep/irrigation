#pragma once
#include <memory>
#include "Logic/StartTime.h"
#include "SampleBase.h"


namespace Dto2ObjectTest {

	typedef ObjectSample<StartTime, StartTimeDTO> StartTimeSample;


	class StartTimeSample1 : public StartTimeSample {
	public:
		StartTimeSample1() : StartTimeSample(StartTime(10, 20), StartTimeDTO(10, 20)) {}
	};

	class StartTimeSample2 : public StartTimeSample {
	public:
		StartTimeSample2() : StartTimeSample(StartTime(11, 25), StartTimeDTO(11, 25)) {}
	};

	class StartTimeSample3 : public StartTimeSample {
	public:
		StartTimeSample3() : StartTimeSample(StartTime(12, 30), StartTimeDTO(12, 30)) {}
	};

	class StartTimeSample4 : public StartTimeSample {
	public:
		StartTimeSample4() : StartTimeSample(StartTime(13, 35), StartTimeDTO(13, 35)) {}
	};
};
