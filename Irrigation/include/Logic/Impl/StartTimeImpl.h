#pragma once
#include <string>
#include "DTO/StartTimeDTO.h"
#include "Logic/StartTime.h"


class StartTimeImpl : public StartTime {
	unsigned hour, minute;

public:
	StartTimeImpl();
	StartTimeImpl(unsigned hour, unsigned minute);
	virtual ~StartTimeImpl();

	virtual bool less(const StartTime& other) const override;

	virtual void set(unsigned hour, unsigned minute) override;	// thows ValueOutOfBoundsException

	virtual unsigned getHours() const override;
	virtual unsigned getMinutes() const override;

	virtual StartTimeDTO toStartTimeDto() const override;
	virtual void updateFromStartTimeDto(const StartTimeDTO& startTimeDTO) override;

	virtual std::string toString() const override;
	virtual std::string toShortString() const override;
};

///////////////////////////////////////////////////////////////////////////////

class StartTimeImplFactory : public StartTimeFactory {
public:
	virtual ~StartTimeImplFactory() = default;
	virtual StartTimePtr create() const override;
};
