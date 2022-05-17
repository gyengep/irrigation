#pragma once
#include <string>
#include "DTO/StartTimeDTO.h"
#include "StartTime.h"


class StartTimeImpl : public StartTime {
	unsigned hour, minute;

public:
	StartTimeImpl();
	StartTimeImpl(StartTimeImpl&&) = delete;
	StartTimeImpl(const StartTimeImpl&) = delete;
	StartTimeImpl(unsigned hour, unsigned minute);
	virtual ~StartTimeImpl();

	virtual void set(unsigned hour, unsigned minute) override;

	virtual unsigned getHours() const override;
	virtual unsigned getMinutes() const override;

	virtual StartTimeDTO toStartTimeDto() const override;
	virtual void updateFromStartTimeDto(const StartTimeDTO& startTimeDTO) override;

	virtual std::string toString() const override;
	virtual std::string toShortString() const override;
};
