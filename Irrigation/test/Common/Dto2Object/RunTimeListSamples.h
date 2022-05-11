#pragma once
#include <list>
#include <memory>
#include "Logic/RunTime.h"
#include "Logic/RunTimeContainer.h"
#include "SampleBase.h"
#include "RunTimeSamples.h"


namespace Dto2ObjectTest {

	typedef ContainerSample<RunTimeContainer, RunTimeDTO> RunTimeListSample;


	class RunTimeListSample1 : public RunTimeListSample {
	public:
		RunTimeListSample1() : RunTimeListSample(
			std::shared_ptr<RunTimeContainer>(new RunTimeContainer({
				RunTimeSample1().getObjectPtr(),
				RunTimeSample2().getObjectPtr(),
				RunTimeSample3().getObjectPtr(),
				RunTimeSample4().getObjectPtr(),
				RunTimeSample5().getObjectPtr(),
				RunTimeSample6().getObjectPtr()
			})),
			std::list<RunTimeDTO>({
				RunTimeSample1().getDto().setId(0),
				RunTimeSample2().getDto().setId(1),
				RunTimeSample3().getDto().setId(2),
				RunTimeSample4().getDto().setId(3),
				RunTimeSample5().getDto().setId(4),
				RunTimeSample6().getDto().setId(5)
			})
		) {}
	};

	class RunTimeListSample2 : public RunTimeListSample {
	public:
		RunTimeListSample2() : RunTimeListSample(
			std::shared_ptr<RunTimeContainer>(new RunTimeContainer({
				RunTimeSample6().getObjectPtr(),
				RunTimeSample5().getObjectPtr(),
				RunTimeSample4().getObjectPtr(),
				RunTimeSample3().getObjectPtr(),
				RunTimeSample2().getObjectPtr(),
				RunTimeSample1().getObjectPtr()
			})),
			std::list<RunTimeDTO>({
				RunTimeSample6().getDto().setId(0),
				RunTimeSample5().getDto().setId(1),
				RunTimeSample4().getDto().setId(2),
				RunTimeSample3().getDto().setId(3),
				RunTimeSample2().getDto().setId(4),
				RunTimeSample1().getDto().setId(5)
			})
		) {}
	};

	class RunTimeListSample3 : public RunTimeListSample {
	public:
		RunTimeListSample3() : RunTimeListSample(
			std::shared_ptr<RunTimeContainer>(new RunTimeContainer({
				RunTimeSample1().getObjectPtr(),
				RunTimeSample3().getObjectPtr(),
				RunTimeSample5().getObjectPtr(),
				RunTimeSample2().getObjectPtr(),
				RunTimeSample4().getObjectPtr(),
				RunTimeSample6().getObjectPtr()
			})),
			std::list<RunTimeDTO>({
				RunTimeSample1().getDto().setId(0),
				RunTimeSample3().getDto().setId(1),
				RunTimeSample5().getDto().setId(2),
				RunTimeSample2().getDto().setId(3),
				RunTimeSample4().getDto().setId(4),
				RunTimeSample6().getDto().setId(5)
			})
		) {}
	};

	class RunTimeListSample4 : public RunTimeListSample {
	public:
		RunTimeListSample4() : RunTimeListSample(
			std::shared_ptr<RunTimeContainer>(new RunTimeContainer({
				RunTimeSample3().getObjectPtr(),
				RunTimeSample1().getObjectPtr(),
				RunTimeSample6().getObjectPtr(),
				RunTimeSample2().getObjectPtr(),
				RunTimeSample4().getObjectPtr(),
				RunTimeSample5().getObjectPtr()
			})),
			std::list<RunTimeDTO>({
				RunTimeSample3().getDto().setId(0),
				RunTimeSample1().getDto().setId(1),
				RunTimeSample6().getDto().setId(2),
				RunTimeSample2().getDto().setId(3),
				RunTimeSample4().getDto().setId(4),
				RunTimeSample5().getDto().setId(5)
			})
		) {}
	};
};
