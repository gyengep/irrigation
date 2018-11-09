#pragma once
#include <array>
#include "DTO/SchedulersDTO.h"
#include "Schedulers/Scheduler.h"



class SpecifiedScheduler : public Scheduler {
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
	typedef std::array<bool, DAY_COUNT> DayArray;

	DayArray days;
	unsigned adjustment;

	void checkIndex(size_t day) const;

public:
	SpecifiedScheduler();
	virtual ~SpecifiedScheduler();

	void setAdjustment(unsigned adjustment);
	void enableDay(size_t day, bool enable);
	bool isDayEnabled(size_t day) const;

	virtual bool isDayScheduled(const std::tm& timeinfo) const override;
	virtual unsigned getAdjustment() const override;


	SpecifiedSchedulerDTO getSpecifiedSchedulerDTO() const;
	void updateFromDTO(const SpecifiedSchedulerDTO& schedulerDTO);

	friend std::string to_string(const SpecifiedScheduler& specifiedScheduler);
};
