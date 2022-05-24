#pragma once
#include <list>
#include <memory>
#include "Logic/Program.h"
#include "Logic/ProgramContainer.h"
#include "Logic/ProgramContainerImpl.h"
#include "SampleBase.h"
#include "ProgramSamples.h"


namespace Dto2ObjectTest {

	typedef ContainerSample<ProgramContainer, ProgramDTO> ProgramListSample;


	class ProgramListSample1 : public ProgramListSample {
	public:
		ProgramListSample1() : ProgramListSample(
			std::make_shared<ProgramContainerImpl>(std::initializer_list<ProgramContainer::value_type>{
			}),
			std::list<ProgramDTO>({
			})
		) {}
	};

	class ProgramListSample2 : public ProgramListSample {
	public:
		ProgramListSample2() : ProgramListSample(
			std::make_shared<ProgramContainerImpl>(std::initializer_list<ProgramContainer::value_type>{
				{ 10, ProgramSample1().getObjectPtr() }
			}),
			std::list<ProgramDTO>({
				ProgramSample1().getDto().setId(10)
			})
		) {}
	};

	class ProgramListSample3 : public ProgramListSample {
	public:
		ProgramListSample3() : ProgramListSample(
			std::make_shared<ProgramContainerImpl>(std::initializer_list<ProgramContainer::value_type>{
				{ 100, ProgramSample1().getObjectPtr() },
				{ 101, ProgramSample2().getObjectPtr() },
				{ 102, ProgramSample3().getObjectPtr() }
			}),
			std::list<ProgramDTO>({
				ProgramSample1().getDto().setId(100),
				ProgramSample2().getDto().setId(101),
				ProgramSample3().getDto().setId(102)
			})
		) {}
	};

	class ProgramListSample4 : public ProgramListSample {
	public:
		ProgramListSample4() : ProgramListSample(
			std::make_shared<ProgramContainerImpl>(std::initializer_list<ProgramContainer::value_type>{
				{ 200, ProgramSample4().getObjectPtr() },
				{ 300, ProgramSample3().getObjectPtr() },
				{ 400, ProgramSample1().getObjectPtr() },
				{ 500, ProgramSample2().getObjectPtr() }
			}),
			std::list<ProgramDTO>({
				ProgramSample4().getDto().setId(200),
				ProgramSample3().getDto().setId(300),
				ProgramSample1().getDto().setId(400),
				ProgramSample2().getDto().setId(500)
			})
		) {}
	};
};
