#pragma once
#include <chrono>
#include <string>
#include "DTO/RunTimeDto.h"
#include "Logic/RunTime.h"


class RunTimeImpl : public RunTime {
	static const std::chrono::hours maxTime;

	std::chrono::seconds time;

public:
	RunTimeImpl();
	RunTimeImpl(const std::chrono::seconds& time);
	virtual ~RunTimeImpl() = default;

	virtual std::chrono::seconds get() const override;
	virtual void set(const std::chrono::seconds& time) override;	// throws ValueOutOfBoundsException

	virtual RunTimeDto toRunTimeDto() const override;
	virtual void updateFromRunTimeDto(const RunTimeDto& runTimeDto) override;

	virtual std::string toString() const override;
	virtual std::chrono::seconds toDuration() const override;
};

///////////////////////////////////////////////////////////////////////////////

class RunTimeImplFactory : public RunTimeFactory {
public:
	virtual ~RunTimeImplFactory() = default;
	virtual RunTimePtr create() const override;
};
