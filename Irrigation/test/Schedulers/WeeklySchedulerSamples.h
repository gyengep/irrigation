#pragma once
#include <memory>
#include "Schedulers/WeeklyScheduler.h"


namespace LogicTest {

	class WeeklySchedulerSample {
	protected:
		std::shared_ptr<WeeklyScheduler> weeklyScheduler;
		WeeklySchedulerDTO weeklySchedulerDto;

	public:
		WeeklySchedulerSample(unsigned adjustment, std::array<bool, 7> days) :
			weeklyScheduler(new WeeklyScheduler())
		{
			weeklyScheduler->setAdjustment(adjustment);

			for (size_t i = 0; i < days.size(); ++i) {
				weeklyScheduler->enableDay(i, days[i]);
			}

			weeklySchedulerDto.setAdjustment(adjustment);
			weeklySchedulerDto.setValues(std::list<bool>(days.begin(), days.end()));
		}

		virtual ~WeeklySchedulerSample() = default;

		std::shared_ptr<WeeklyScheduler> getWeeklyScheduler() { return weeklyScheduler; }
		const std::shared_ptr<WeeklyScheduler> getWeeklyScheduler() const { return weeklyScheduler; }

		WeeklySchedulerDTO& getWeeklySchedulerDto() { return weeklySchedulerDto; }
		const WeeklySchedulerDTO& getWeeklySchedulerDto() const { return weeklySchedulerDto; }
	};


	class WeeklySchedulerSample1 : public WeeklySchedulerSample {
	public:
		WeeklySchedulerSample1() : WeeklySchedulerSample(
			10, std::array<bool, 7>({ true, false, true, false, true, false, true }))
		{}
	};

	class WeeklySchedulerSample2 : public WeeklySchedulerSample {
	public:
		WeeklySchedulerSample2() : 	WeeklySchedulerSample(
			20, std::array<bool, 7>({ false, true, false, true, false, true, false }))
		{}
	};

	class WeeklySchedulerSample3 : public WeeklySchedulerSample {
	public:
		WeeklySchedulerSample3() : 	WeeklySchedulerSample(
			30, std::array<bool, 7>({ true, true, false, false, true, false, true }))
		{}
	};

	class WeeklySchedulerSample4 : public WeeklySchedulerSample {
	public:
		WeeklySchedulerSample4() : 	WeeklySchedulerSample(
			40, std::array<bool, 7>({ false, false, false, false, true, false, true }))
		{}
	};
};
