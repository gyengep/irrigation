#pragma once
#include <memory>
#include "Schedulers/HotWeatherScheduler.h"
#include "SampleBase.h"


namespace Dto2ObjectTest {

	typedef ObjectSample<HotWeatherScheduler, HotWeatherSchedulerDTO> HotWeatherSchedulerSample;


	class HotWeatherSchedulerSample1 : public HotWeatherSchedulerSample {
	public:
		HotWeatherSchedulerSample1() : HotWeatherSchedulerSample(
			std::make_shared<HotWeatherScheduler>(nullptr, std::chrono::seconds(60), 35.0f),
			HotWeatherSchedulerDTO(60, 35.0f)
		) {}
	};

	class HotWeatherSchedulerSample2 : public HotWeatherSchedulerSample {
	public:
		HotWeatherSchedulerSample2() : HotWeatherSchedulerSample(
			std::make_shared<HotWeatherScheduler>(nullptr, std::chrono::seconds(120), 30.0f),
			HotWeatherSchedulerDTO(120, 30.0f)
		) {}
	};

	class HotWeatherSchedulerSample3 : public HotWeatherSchedulerSample {
	public:
		HotWeatherSchedulerSample3() : HotWeatherSchedulerSample(
			std::make_shared<HotWeatherScheduler>(nullptr, std::chrono::seconds(180), 33.0f),
			HotWeatherSchedulerDTO(180, 33.0f)
		) {}
	};

	class HotWeatherSchedulerSample4 : public HotWeatherSchedulerSample {
	public:
		HotWeatherSchedulerSample4() : HotWeatherSchedulerSample(
			std::make_shared<HotWeatherScheduler>(nullptr, std::chrono::seconds(150), 25.0f),
			HotWeatherSchedulerDTO(150, 25.0f)
		) {}
	};
};
