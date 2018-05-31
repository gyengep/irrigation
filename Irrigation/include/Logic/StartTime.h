#pragma once
#include <string>


class StartTime {
	static const unsigned maxSeconds = 60 * 60 * 24;

	// disable copy constructor and copy operator
	StartTime(const StartTime&);
	StartTime& operator= (const StartTime&);

	unsigned secOfDay;
	bool enabled;

public:
	StartTime();
	StartTime(unsigned hour, unsigned min, unsigned sec);
	virtual ~StartTime();

	bool operator< (const StartTime& other) const;
	bool operator== (const StartTime& other) const;

	bool isEnabled() const { return enabled; }
	void enable(bool enabled) { this->enabled = enabled; }

	virtual unsigned getValue() const;
	virtual void setValue(unsigned secOfDay);

	std::string toString() const;
	void fromString(const std::string& text);
};
