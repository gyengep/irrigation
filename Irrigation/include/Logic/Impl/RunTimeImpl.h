#pragma once
#include <chrono>
#include <string>
#include "DTO/RunTimeDTO.h"
#include "Logic/RunTime.h"


class RunTimeImpl : public RunTime {
	static const unsigned maxSeconds = 60 * 60 * 24;

	unsigned seconds;

public:
	RunTimeImpl();
	RunTimeImpl(unsigned seconds);
	RunTimeImpl(const std::chrono::seconds& seconds);
	virtual ~RunTimeImpl() = default;

	virtual unsigned getSeconds() const override;
	virtual void setSeconds(unsigned seconds) override;

	virtual RunTimeDTO toRunTimeDto() const override;
	virtual void updateFromRunTimeDto(const RunTimeDTO& runTimeDTO) override;

	virtual std::string toString() const override;
	virtual std::chrono::seconds toDuration() const override;
};

///////////////////////////////////////////////////////////////////////////////

class RunTimeImplFactory : public RunTimeFactory {
public:
	virtual ~RunTimeImplFactory() = default;
	virtual RunTimePtr create() const override;
};
