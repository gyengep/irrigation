#pragma once
#include <list>
#include <memory>
#include "Logic/StartTime.h"
#include "Logic/StartTimeContainer.h"
#include "SampleBase.h"
#include "StartTimeSamples.h"


namespace Dto2ObjectTest {

	typedef ContainerSample<StartTimeContainer, StartTimeDTO> StartTimeListSample;


	class StartTimeListSample1 : public StartTimeListSample {
	public:
		StartTimeListSample1() : StartTimeListSample(
			std::shared_ptr<StartTimeContainer>(new StartTimeContainer({
			})),
			std::list<StartTimeDTO>({
			})
		) {}
	};

	class StartTimeListSample2 : public StartTimeListSample {
	public:
		StartTimeListSample2() : StartTimeListSample(
			std::shared_ptr<StartTimeContainer>(new StartTimeContainer({
				{ 10, StartTimeSample1().getObjectPtr() }
			})),
			std::list<StartTimeDTO>({
				StartTimeSample1().getDto().setId(10)
			})
		) {}
	};

	class StartTimeListSample3 : public StartTimeListSample {
	public:
		StartTimeListSample3() : StartTimeListSample(
			std::shared_ptr<StartTimeContainer>(new StartTimeContainer({
				{ 100, StartTimeSample1().getObjectPtr() },
				{ 101, StartTimeSample2().getObjectPtr() },
				{ 102, StartTimeSample3().getObjectPtr() }
			})),
			std::list<StartTimeDTO>({
				StartTimeSample1().getDto().setId(100),
				StartTimeSample2().getDto().setId(101),
				StartTimeSample3().getDto().setId(102)
			})
		) {}
	};

	class StartTimeListSample4 : public StartTimeListSample {
	public:
		StartTimeListSample4() : StartTimeListSample(
			std::shared_ptr<StartTimeContainer>(new StartTimeContainer({
				{ 200, StartTimeSample4().getObjectPtr() },
				{ 300, StartTimeSample3().getObjectPtr() },
				{ 400, StartTimeSample1().getObjectPtr() },
				{ 500, StartTimeSample2().getObjectPtr() }
			})),
			std::list<StartTimeDTO>({
				StartTimeSample4().getDto().setId(200),
				StartTimeSample3().getDto().setId(300),
				StartTimeSample1().getDto().setId(400),
				StartTimeSample2().getDto().setId(500)
			})
		) {}
	};
};
