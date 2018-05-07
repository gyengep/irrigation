#pragma once


class StartTime {
	static const unsigned maxSeconds = 60 * 60 * 24;

	unsigned secOfDay;
	bool enabled;

public:
	StartTime();
	virtual ~StartTime();

	StartTime& operator= (const StartTime& other);
	bool operator< (const StartTime& other) const;

	bool isEnabled() const { return enabled; }
	void enable(bool enabled) { this->enabled = enabled; }

	virtual unsigned getValue() const;
	virtual void setValue(unsigned secOfDay);
};
