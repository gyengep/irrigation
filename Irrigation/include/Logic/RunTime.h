#pragma once
#include <chrono>
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
	RunTime(const std::chrono::seconds& seconds);
	virtual ~RunTime() = default;

	RunTime& operator= (RunTime&&) = delete;
	RunTime& operator= (const RunTime&) = delete;
	bool operator== (const RunTime& other) const;

	unsigned getSeconds() const;
	void setSeconds(unsigned seconds);

	RunTimeDTO toRunTimeDto() const;
	virtual void updateFromRunTimeDto(const RunTimeDTO& runTimeDTO);

	friend std::string to_string(const RunTime& runTime);
	friend std::ostream& operator<<(std::ostream& os, const RunTime& runTime);
};
