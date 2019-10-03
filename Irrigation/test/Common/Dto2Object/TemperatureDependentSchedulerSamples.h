#pragma once
#include <memory>
#include "Schedulers/TemperatureDependentScheduler.h"
#include "SampleBase.h"


namespace Dto2ObjectTest {

	typedef ObjectSample<TemperatureDependentScheduler, TemperatureDependentSchedulerDTO> TemperatureDependentSchedulerSample;


	class TemperatureDependentSchedulerSample1 : public TemperatureDependentSchedulerSample {
	public:
		TemperatureDependentSchedulerSample1() : TemperatureDependentSchedulerSample(
				TemperatureDependentScheduler(1.0f, 1.1f, 1.2f, 1.3f, 1.4f, 10, 20, 30),
				TemperatureDependentSchedulerDTO(1.0f, 1.1f, 1.2f, 1.3f, 1.4f, 10, 20, 30)
			)
		{}
	};

	class TemperatureDependentSchedulerSample2 : public TemperatureDependentSchedulerSample {
	public:
		TemperatureDependentSchedulerSample2() : TemperatureDependentSchedulerSample(
				TemperatureDependentScheduler(2.0f, 2.1f, 2.2f, 2.3f, 2.4f, 15, 25, 35),
				TemperatureDependentSchedulerDTO(2.0f, 2.1f, 2.2f, 2.3f, 2.4f, 15, 25, 35)
			)
		{}
	};

	class TemperatureDependentSchedulerSample3 : public TemperatureDependentSchedulerSample {
	public:
		TemperatureDependentSchedulerSample3() : TemperatureDependentSchedulerSample(
				TemperatureDependentScheduler(12.0f, 12.1f, 12.2f, 12.3f, 12.4f, 25, 26, 27),
				TemperatureDependentSchedulerDTO(12.0f, 12.1f, 12.2f, 12.3f, 12.4f, 25, 26, 27)
			)
		{}
	};

	class TemperatureDependentSchedulerSample4 : public TemperatureDependentSchedulerSample {
	public:
		TemperatureDependentSchedulerSample4() : TemperatureDependentSchedulerSample(
				TemperatureDependentScheduler(13.0f, 14.1f, 15.2f, 16.3f, 16.4f, 25, 26, 27),
				TemperatureDependentSchedulerDTO(13.0f, 14.1f, 15.2f, 16.3f, 16.4f, 25, 26, 27)
			)
		{}
	};
};
