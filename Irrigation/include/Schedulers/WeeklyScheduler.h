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

	unsigned adjustment;
	std::array<bool, 7> days;

	void checkIndex(size_t day) const;

public:
	WeeklyScheduler();
	WeeklyScheduler(WeeklyScheduler&&) = default;
	WeeklyScheduler(const WeeklyScheduler&);
	WeeklyScheduler(unsigned adjustment, const std::array<bool, 7>& days);
	virtual ~WeeklyScheduler();

	WeeklyScheduler& operator= (WeeklyScheduler&&) = delete;
	WeeklyScheduler& operator= (const WeeklyScheduler&) = delete;
	bool operator== (const WeeklyScheduler& other) const;

	void setAdjustment(unsigned adjustment);
	void enableDay(size_t day, bool enable);
	bool isDayEnabled(size_t day) const;

	virtual bool isDayScheduled(const std::tm& timeinfo) const override;
	virtual unsigned getAdjustment() const override;

	WeeklySchedulerDTO toWeeklySchedulerDto() const;
	virtual void updateFromWeeklySchedulerDto(const WeeklySchedulerDTO& schedulerDTO);

	friend std::string to_string(const WeeklyScheduler& weeklyScheduler);
	friend std::ostream& operator<<(std::ostream& os, const WeeklyScheduler& weeklyScheduler);
};
