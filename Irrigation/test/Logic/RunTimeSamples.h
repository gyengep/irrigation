#pragma once
#include <memory>
#include "Logic/RunTime.h"


namespace LogicTest {

	class RunTimeSample {
	protected:
		std::shared_ptr<RunTime> runTime;
		RunTimeDTO runTimeDto;

	public:
		RunTimeSample(unsigned minutes, unsigned seconds) :
			runTime(new RunTime(60 * minutes + seconds))
		{
			runTimeDto.setMinutes(minutes);
			runTimeDto.setSeconds(seconds);
		}

		virtual ~RunTimeSample() = default;

		std::shared_ptr<RunTime> getRunTime() { return runTime; }
		const std::shared_ptr<RunTime> getRunTime() const { return runTime; }

		RunTimeDTO& getRunTimeDto() { return runTimeDto; }
		const RunTimeDTO& getRunTimeDto() const { return runTimeDto; }
	};


	class RunTimeSample1 : public RunTimeSample {
	public:
		RunTimeSample1() : RunTimeSample(1, 10) {}
	};

	class RunTimeSample2 : public RunTimeSample {
	public:
		RunTimeSample2() : RunTimeSample(2, 15) {}
	};

	class RunTimeSample3 : public RunTimeSample {
	public:
		RunTimeSample3() : RunTimeSample(3, 20) {}
	};

	class RunTimeSample4 : public RunTimeSample {
	public:
		RunTimeSample4() : RunTimeSample(4, 30) {}
	};

	class RunTimeSample5 : public RunTimeSample {
	public:
		RunTimeSample5() : RunTimeSample(5, 40) {}
	};

	class RunTimeSample6 : public RunTimeSample {
	public:
		RunTimeSample6() : RunTimeSample(10, 50) {}
	};
};
