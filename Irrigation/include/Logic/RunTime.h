#pragma once
#include <string>
#include "DTO/RunTimeDTO.h"


class RunTime {
	static const unsigned maxSeconds = 60 * 60 * 24;

	unsigned seconds;

public:
	RunTime();
	RunTime(RunTime&&) = default;
	RunTime(const RunTime&) = default;
	RunTime(unsigned seconds);
	virtual ~RunTime() = default;

	RunTime& operator= (RunTime&&) = default;
	RunTime& operator= (const RunTime&) = default;
	bool operator== (const RunTime& other) const;

	unsigned getSeconds() const;
	void setSeconds(unsigned seconds);

	RunTimeDTO getRunTimeDTO() const;
	void updateFromDTO(const RunTimeDTO& runTimeDTO);

	friend std::string to_string(const RunTime& runTime);
};

///////////////////////////////////////////////////////////////////////////////

class RunTimeFactory {
public:
	virtual ~RunTimeFactory() = default;
	virtual std::shared_ptr<RunTime> createRunTime() const;
};


