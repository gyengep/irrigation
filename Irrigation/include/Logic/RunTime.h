#pragma once
#include <string>
#include "DTO/RunTimeDTO.h"


class RunTime {
	static const unsigned maxSeconds = 60 * 60 * 24;

	unsigned seconds;

public:
	RunTime();
	RunTime(const RunTime& other) = default;
	RunTime(unsigned seconds);
	virtual ~RunTime();

	unsigned getSeconds() const;
	void setSeconds(unsigned seconds);

	RunTimeDTO getRunTimeDTO() const;
	void updateFromDTO(const RunTimeDTO& runTimeDTO);

	RunTime& operator= (const RunTime& other) = default;
	bool operator== (const RunTime& other) const;

	friend std::string to_string(const RunTime& runTime);
};
