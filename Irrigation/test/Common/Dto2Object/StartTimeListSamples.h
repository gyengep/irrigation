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
		StartTimeListSample1() :
			StartTimeListSample(
				StartTimeContainer(),
				std::list<StartTimeDTO>()
			)
		{}
	};

	class StartTimeListSample2 : public StartTimeListSample {
	public:
		StartTimeListSample2() :
			StartTimeListSample(
				StartTimeContainer({
					{ 10, StartTimeSample1().getObject() }
				}),
				std::list<StartTimeDTO>({
					StartTimeSample1().getDto().setId(10)
				})
			)
		{}
	};

	class StartTimeListSample3 : public StartTimeListSample {
	public:
		StartTimeListSample3() :
			StartTimeListSample(
				StartTimeContainer({
					{ 100, StartTimeSample1().getObject() },
					{ 101, StartTimeSample2().getObject() },
					{ 102, StartTimeSample3().getObject() }
				}),
				std::list<StartTimeDTO>({
					StartTimeSample1().getDto().setId(100),
					StartTimeSample2().getDto().setId(101),
					StartTimeSample3().getDto().setId(102)
				})
			)
		{}
	};

	class StartTimeListSample4 : public StartTimeListSample {
	public:
		StartTimeListSample4() :
			StartTimeListSample(
				StartTimeContainer({
					{ 200, StartTimeSample4().getObject() },
					{ 300, StartTimeSample3().getObject() },
					{ 400, StartTimeSample1().getObject() },
					{ 500, StartTimeSample2().getObject() }
				}),
				std::list<StartTimeDTO>({
					StartTimeSample4().getDto().setId(200),
					StartTimeSample3().getDto().setId(300),
					StartTimeSample1().getDto().setId(400),
					StartTimeSample2().getDto().setId(500)
				})
			)
		{}
	};
};
