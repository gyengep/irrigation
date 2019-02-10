#pragma once
#include <memory>
#include "Logic/RunTime.h"
#include "SampleBase.h"


namespace Dto2ObjectTest {

	typedef ObjectSample<RunTime, RunTimeDTO> RunTimeSample;


	class RunTimeSample1 : public RunTimeSample {
	public:
		RunTimeSample1() : RunTimeSample(RunTime(60 * 1 + 10), RunTimeDTO(1, 10)) {}
	};

	class RunTimeSample2 : public RunTimeSample {
	public:
		RunTimeSample2() : RunTimeSample(RunTime(60 * 2 + 15), RunTimeDTO(2, 15)) {}
	};

	class RunTimeSample3 : public RunTimeSample {
	public:
		RunTimeSample3() : RunTimeSample(RunTime(60 * 3 + 20), RunTimeDTO(3, 20)) {}
	};

	class RunTimeSample4 : public RunTimeSample {
	public:
		RunTimeSample4() : RunTimeSample(RunTime(60 * 4 + 30), RunTimeDTO(4, 30)) {}
	};

	class RunTimeSample5 : public RunTimeSample {
	public:
		RunTimeSample5() : RunTimeSample(RunTime(60 * 5 + 40), RunTimeDTO(5, 40)) {}
	};

	class RunTimeSample6 : public RunTimeSample {
	public:
		RunTimeSample6() : RunTimeSample(RunTime(60 * 10 + 50), RunTimeDTO(10, 50)) {}
	};
};
