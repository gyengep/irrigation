#pragma once
#include <chrono>
#include "RunTime.h"
#include "DTO/RunTimeDTO.h"


class RunTimeImpl : public RunTime {
	static const unsigned maxSeconds = 60 * 60 * 24;

	unsigned milliSeconds;

public:
	RunTimeImpl();
	RunTimeImpl(RunTimeImpl&&) = delete;
	RunTimeImpl(const RunTimeImpl&) = delete;
	RunTimeImpl(unsigned seconds);
	RunTimeImpl(const std::chrono::seconds& seconds);
	virtual ~RunTimeImpl() = default;

	virtual unsigned getSeconds() const override;
	virtual unsigned getMilliSeconds() const override;
	virtual void setSeconds(unsigned seconds) override;
	virtual void setMilliSeconds(unsigned milliSeconds) override;

	virtual RunTimeDTO toRunTimeDto() const override;
	virtual void updateFromRunTimeDto(const RunTimeDTO& runTimeDTO) override;
};
