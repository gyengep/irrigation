#pragma once
#include <string>


class RunTime {
	static const unsigned maxSeconds = 60 * 60 * 2;

	// disable copy constructor and copy operator
	RunTime(const RunTime&);
	RunTime& operator= (const RunTime&);

	unsigned seconds;

public:
	RunTime();
	virtual ~RunTime();

	virtual unsigned getValue() const;
	virtual void setValue(unsigned seconds);

	std::string toString() const;
	void fromString(const std::string& text);
};
