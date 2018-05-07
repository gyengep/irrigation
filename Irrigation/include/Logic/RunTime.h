#pragma once


class RunTime {
	static const unsigned maxSeconds = 60 * 60 * 2;

	unsigned seconds;

public:
	RunTime();
	virtual ~RunTime();

	RunTime& operator= (const RunTime& other);
	bool operator== (const RunTime& other) const;
	bool operator< (const RunTime& other) const;

	virtual unsigned getValue() const;
	virtual void setValue(unsigned seconds);
};
