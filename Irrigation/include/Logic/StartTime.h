#pragma once
#include <string>
#include "ReaderWriter/StartTimeDTO.h"


class StartTime {
	static const unsigned maxSeconds = 60 * 60 * 24;

	// disable copy constructor and copy operator
	StartTime(const StartTime&);
	StartTime& operator= (const StartTime&);

	unsigned secOfDay;

public:
	StartTime();
	StartTime(unsigned hour, unsigned min, unsigned sec);
	virtual ~StartTime();

	bool operator< (const StartTime& other) const;
	bool operator== (const StartTime& other) const;

	virtual unsigned getValue() const;
	virtual void setValue(unsigned secOfDay);

	StartTimeDTO getStartTimeDTO() const;
};
