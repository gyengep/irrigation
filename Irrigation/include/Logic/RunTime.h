#pragma once
#include <string>
#include "DTO/RunTimeDTO.h"


class RunTime {
	static const unsigned maxSeconds = 60 * 60 * 24;

	// disable copy constructor and copy operator
	RunTime(const RunTime&);
	RunTime& operator= (const RunTime&);

	unsigned seconds;

public:
	RunTime();
	virtual ~RunTime();

	unsigned getSeconds() const;
	void setSeconds(unsigned seconds);

	RunTimeDTO getRunTimeDTO() const;
	void updateFromDTO(const RunTimeDTO& runTimeDTO);

	friend std::string to_string(const RunTime& runTime);
};
