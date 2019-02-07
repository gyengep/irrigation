#pragma once
#include <memory>
#include "Schedulers/PeriodicScheduler.h"


namespace LogicTest {

	class PeriodicSchedulerSample {
	protected:
		std::shared_ptr<PeriodicScheduler> periodicScheduler;
		PeriodicSchedulerDTO periodicSchedulerDto;

	public:
		PeriodicSchedulerSample(unsigned adjustment, std::vector<bool> days,
				unsigned year, unsigned month, unsigned day) :
			periodicScheduler(new PeriodicScheduler())
		{
			periodicScheduler->setAdjustment(adjustment);
			periodicScheduler->setPeriod(days.size());
			periodicScheduler->setPeriodStartDate(year, month, day);

			for (size_t i = 0; i < days.size(); ++i) {
				periodicScheduler->enableDay(i, days[i]);
			}

			periodicSchedulerDto.setAdjustment(adjustment);
			periodicSchedulerDto.setValues(std::list<bool>(days.begin(), days.end()));
			periodicSchedulerDto.setPeriodStartYear(year);
			periodicSchedulerDto.setPeriodStartMonth(month);
			periodicSchedulerDto.setPeriodStartDay(day);
		}

		virtual ~PeriodicSchedulerSample() = default;

		std::shared_ptr<PeriodicScheduler> getPeriodicScheduler() { return periodicScheduler; }
		const std::shared_ptr<PeriodicScheduler> getPeriodicScheduler() const { return periodicScheduler; }

		PeriodicSchedulerDTO& getPeriodicSchedulerDto() { return periodicSchedulerDto; }
		const PeriodicSchedulerDTO& getPeriodicSchedulerDto() const { return periodicSchedulerDto; }
	};


	class PeriodicSchedulerSample1 : public PeriodicSchedulerSample {
	public:
		PeriodicSchedulerSample1() :
			PeriodicSchedulerSample(
					10,
					std::vector<bool>(),
					2018, 1, 10)
		{}
	};

	class PeriodicSchedulerSample2 : public PeriodicSchedulerSample {
	public:
		PeriodicSchedulerSample2() :
			PeriodicSchedulerSample(
					20,
					std::vector<bool>({ true }),
					2017, 2, 11)
		{}
	};

	class PeriodicSchedulerSample3 : public PeriodicSchedulerSample {
	public:
		PeriodicSchedulerSample3() :
			PeriodicSchedulerSample(
					30,
					std::vector<bool>({ false, true, false, true }),
					2019, 3, 12)
		{}
	};

	class PeriodicSchedulerSample4 : public PeriodicSchedulerSample {
	public:
		PeriodicSchedulerSample4() :
			PeriodicSchedulerSample(
					40,
					std::vector<bool>({ true, true, false, false, true, false, true }),
					2000, 4, 13)
		{}
	};
};
