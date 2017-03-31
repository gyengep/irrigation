#pragma once


class StartTime {
	int hour;
	int min;

public:
	StartTime(int hour, int min);

	StartTime& operator= (const StartTime& other);
	bool operator== (const StartTime& other) const;
	bool operator< (const StartTime& other) const;

	int getHour() const { return hour; }
	int getMin() const { return min; }
};

std::ostream& operator<< (std::ostream& os, const StartTime& startTime);
