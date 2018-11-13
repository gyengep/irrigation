#pragma once
#include <vector>
#include "DTO/SchedulersDTO.h"
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
	static const unsigned DAY_COUNT = 7;
	typedef std::vector<bool> DayArray;

	DayArray days;
	unsigned adjustment;

	void checkIndex(size_t day) const;

public:
	WeeklyScheduler();
	virtual ~WeeklyScheduler();

	void setAdjustment(unsigned adjustment);
	void enableDay(size_t day, bool enable);
	bool isDayEnabled(size_t day) const;

	virtual bool isDayScheduled(const std::tm& timeinfo) const override;
	virtual unsigned getAdjustment() const override;

	WeeklySchedulerDTO getWeeklySchedulerDTO() const;
	void updateFromDTO(const WeeklySchedulerDTO& schedulerDTO);

	friend std::string to_string(const WeeklyScheduler& weeklyScheduler);
};
