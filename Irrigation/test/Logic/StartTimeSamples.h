#pragma once
#include <memory>
#include "Logic/StartTime.h"


namespace LogicTest {

	class StartTimeSample {
	protected:
		std::shared_ptr<StartTime> startTime;
		StartTimeDTO startTimeDto;

	public:
		StartTimeSample(unsigned hours, unsigned minutes) : startTime(new StartTime(hours, minutes)) {
			startTimeDto.setHours(hours);
			startTimeDto.setMinutes(minutes);
		}

		virtual ~StartTimeSample() = default;

		std::shared_ptr<StartTime> getStartTime() { return startTime; }
		const std::shared_ptr<StartTime> getStartTime() const { return startTime; }

		StartTimeDTO& getStartTimeDto() { return startTimeDto; }
		const StartTimeDTO& getStartTimeDto() const { return startTimeDto; }
	};


	class StartTimeSample1 : public StartTimeSample {
	public:
		StartTimeSample1() : StartTimeSample(10, 20) {}
	};

	class StartTimeSample2 : public StartTimeSample {
	public:
		StartTimeSample2() : StartTimeSample(11, 25) {}
	};

	class StartTimeSample3 : public StartTimeSample {
	public:
		StartTimeSample3() : StartTimeSample(12, 30) {}
	};

	class StartTimeSample4 : public StartTimeSample {
	public:
		StartTimeSample4() : StartTimeSample(13, 35) {}
	};
};
