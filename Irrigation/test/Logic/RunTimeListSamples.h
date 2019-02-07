#pragma once
#include <list>
#include <memory>
#include "Logic/RunTime.h"
#include "Logic/RunTimeContainer.h"
#include "RunTimeSamples.h"


namespace LogicTest {

	class RunTimeListSample {
	protected:
		std::shared_ptr<RunTimeContainer> runTimeContainer;
		std::list<RunTimeDTO> runTimeDtoList;

	public:
		RunTimeListSample() = default;
		virtual ~RunTimeListSample() = default;

		std::shared_ptr<RunTimeContainer> getRunTimeContainer() { return runTimeContainer; }
		const std::shared_ptr<RunTimeContainer> getRunTimeContainer() const { return runTimeContainer; }

		std::list<RunTimeDTO>& getRunTimeDtoList() { return runTimeDtoList; }
		const std::list<RunTimeDTO>& getRunTimeDtoList() const { return runTimeDtoList; }
	};


	class RunTimeListSample1 : public RunTimeListSample {
	public:
		RunTimeListSample1() : RunTimeListSample() {
			runTimeContainer.reset(new RunTimeContainer({
				*RunTimeSample1().getRunTime(),
				*RunTimeSample2().getRunTime(),
				*RunTimeSample3().getRunTime(),
				*RunTimeSample4().getRunTime(),
				*RunTimeSample5().getRunTime(),
				*RunTimeSample6().getRunTime()
			}));

			runTimeDtoList.push_back(RunTimeSample1().getRunTimeDto().setId(0));
			runTimeDtoList.push_back(RunTimeSample2().getRunTimeDto().setId(1));
			runTimeDtoList.push_back(RunTimeSample3().getRunTimeDto().setId(2));
			runTimeDtoList.push_back(RunTimeSample4().getRunTimeDto().setId(3));
			runTimeDtoList.push_back(RunTimeSample5().getRunTimeDto().setId(4));
			runTimeDtoList.push_back(RunTimeSample6().getRunTimeDto().setId(5));
		}
	};

	class RunTimeListSample2 : public RunTimeListSample {
	public:
		RunTimeListSample2() {
			runTimeContainer.reset(new RunTimeContainer({
				*RunTimeSample6().getRunTime(),
				*RunTimeSample5().getRunTime(),
				*RunTimeSample4().getRunTime(),
				*RunTimeSample3().getRunTime(),
				*RunTimeSample2().getRunTime(),
				*RunTimeSample1().getRunTime()
			}));

			runTimeDtoList.push_back(RunTimeSample6().getRunTimeDto().setId(0));
			runTimeDtoList.push_back(RunTimeSample5().getRunTimeDto().setId(1));
			runTimeDtoList.push_back(RunTimeSample4().getRunTimeDto().setId(2));
			runTimeDtoList.push_back(RunTimeSample3().getRunTimeDto().setId(3));
			runTimeDtoList.push_back(RunTimeSample2().getRunTimeDto().setId(4));
			runTimeDtoList.push_back(RunTimeSample1().getRunTimeDto().setId(5));
		}
	};
};
