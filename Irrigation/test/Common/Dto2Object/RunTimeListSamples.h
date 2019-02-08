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
		RunTimeListSample1() :
			RunTimeListSample(
				RunTimeContainer({
					*RunTimeSample1().getObject(),
					*RunTimeSample2().getObject(),
					*RunTimeSample3().getObject(),
					*RunTimeSample4().getObject(),
					*RunTimeSample5().getObject(),
					*RunTimeSample6().getObject()
				}),
				std::list<RunTimeDTO>({
					RunTimeSample1().getDto().setId(0),
					RunTimeSample2().getDto().setId(1),
					RunTimeSample3().getDto().setId(2),
					RunTimeSample4().getDto().setId(3),
					RunTimeSample5().getDto().setId(4),
					RunTimeSample6().getDto().setId(5)
				})
			)
		{}
	};

	class RunTimeListSample2 : public RunTimeListSample {
	public:
		RunTimeListSample2() :
			RunTimeListSample(
				RunTimeContainer({
					*RunTimeSample6().getObject(),
					*RunTimeSample5().getObject(),
					*RunTimeSample4().getObject(),
					*RunTimeSample3().getObject(),
					*RunTimeSample2().getObject(),
					*RunTimeSample1().getObject()
				}),
				std::list<RunTimeDTO>({
					RunTimeSample6().getDto().setId(0),
					RunTimeSample5().getDto().setId(1),
					RunTimeSample4().getDto().setId(2),
					RunTimeSample3().getDto().setId(3),
					RunTimeSample2().getDto().setId(4),
					RunTimeSample1().getDto().setId(5)
				})
			)
		{}
	};

	class RunTimeListSample3 : public RunTimeListSample {
	public:
		RunTimeListSample3() :
			RunTimeListSample(
				RunTimeContainer({
					*RunTimeSample1().getObject(),
					*RunTimeSample3().getObject(),
					*RunTimeSample5().getObject(),
					*RunTimeSample2().getObject(),
					*RunTimeSample4().getObject(),
					*RunTimeSample6().getObject()
				}),
				std::list<RunTimeDTO>({
					RunTimeSample1().getDto().setId(0),
					RunTimeSample3().getDto().setId(1),
					RunTimeSample5().getDto().setId(2),
					RunTimeSample2().getDto().setId(3),
					RunTimeSample4().getDto().setId(4),
					RunTimeSample6().getDto().setId(5)
				})
			)
		{}
	};

	class RunTimeListSample4 : public RunTimeListSample {
	public:
		RunTimeListSample4() :
			RunTimeListSample(
				RunTimeContainer({
					*RunTimeSample3().getObject(),
					*RunTimeSample1().getObject(),
					*RunTimeSample6().getObject(),
					*RunTimeSample2().getObject(),
					*RunTimeSample4().getObject(),
					*RunTimeSample5().getObject()
				}),
				std::list<RunTimeDTO>({
					RunTimeSample3().getDto().setId(0),
					RunTimeSample1().getDto().setId(1),
					RunTimeSample6().getDto().setId(2),
					RunTimeSample2().getDto().setId(3),
					RunTimeSample4().getDto().setId(4),
					RunTimeSample5().getDto().setId(5)
				})
			)
		{}
	};
};
