#pragma once
#include <memory>
#include "Schedulers/TemperatureDependentSchedulerImpl.h"
#include "SampleBase.h"


namespace Dto2ObjectTest {

	typedef ObjectSample<TemperatureDependentSchedulerImpl, TemperatureDependentSchedulerDTO> TemperatureDependentSchedulerSample;


	class TemperatureDependentSchedulerSample1 : public TemperatureDependentSchedulerSample {
	public:
		TemperatureDependentSchedulerSample1() : TemperatureDependentSchedulerSample(
			std::make_shared<TemperatureDependentSchedulerImpl>(nullptr, nullptr, 1.0f, 10, 20, 30),
			TemperatureDependentSchedulerDTO(1.0f, 10, 20, 30)
		) {}
	};

	class TemperatureDependentSchedulerSample2 : public TemperatureDependentSchedulerSample {
	public:
		TemperatureDependentSchedulerSample2() : TemperatureDependentSchedulerSample(
			std::make_shared<TemperatureDependentSchedulerImpl>(nullptr, nullptr, 2.0f, 15, 25, 35),
			TemperatureDependentSchedulerDTO(2.0f, 15, 25, 35)
		) {}
	};

	class TemperatureDependentSchedulerSample3 : public TemperatureDependentSchedulerSample {
	public:
		TemperatureDependentSchedulerSample3() : TemperatureDependentSchedulerSample(
			std::make_shared<TemperatureDependentSchedulerImpl>(nullptr, nullptr, 12.0f, 25, 26, 27),
			TemperatureDependentSchedulerDTO(12.0f, 25, 26, 27)
		) {}
	};

	class TemperatureDependentSchedulerSample4 : public TemperatureDependentSchedulerSample {
	public:
		TemperatureDependentSchedulerSample4() : TemperatureDependentSchedulerSample(
			std::make_shared<TemperatureDependentSchedulerImpl>(nullptr, nullptr, 13.0f, 25, 26, 27),
			TemperatureDependentSchedulerDTO(13.0f, 25, 26, 27)
		) {}
	};
};
