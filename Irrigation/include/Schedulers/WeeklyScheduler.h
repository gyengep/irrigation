#pragma once
#include <array>
#include "DTO/WeeklySchedulerDTO.h"
#include "Schedulers/Scheduler.h"



class WeeklyScheduler : public Scheduler {
public:

	enum {
		MONDAY,
		TUESDAY,
		WEDNESDAY,
		THURSDAY,
		FRIDAY,
		SATURDAY,
		SUNDAY
	};

private:

	std::array<bool, 7> days;
	bool isScheduled;

	void checkIndex(size_t day) const;
	bool onProcess(const std::tm& timeinfo) const;


public:
	WeeklyScheduler();
	WeeklyScheduler(WeeklyScheduler&&) = default;
	WeeklyScheduler(const WeeklyScheduler&);
	WeeklyScheduler(const std::array<bool, 7>& days);
	virtual ~WeeklyScheduler();

	WeeklyScheduler& operator= (WeeklyScheduler&&) = delete;
	WeeklyScheduler& operator= (const WeeklyScheduler&) = delete;
	bool operator== (const WeeklyScheduler& other) const;

	void enableDay(size_t day, bool enable);
	bool isDayEnabled(size_t day) const;

	virtual unsigned onProcess(const std::time_t rawtime) override;

	WeeklySchedulerDTO toWeeklySchedulerDto() const;
	virtual void updateFromWeeklySchedulerDto(const WeeklySchedulerDTO& schedulerDTO);

	friend std::string to_string(const WeeklyScheduler& weeklyScheduler);
	friend std::ostream& operator<<(std::ostream& os, const WeeklyScheduler& weeklyScheduler);
};
