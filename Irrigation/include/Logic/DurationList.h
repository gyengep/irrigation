#pragma once
#include <chrono>
#include <list>
#include <string>
#include "Utils/OstreamInsert.h"


class DurationList : public std::list<std::chrono::seconds> {
public:
	DurationList() = default;
	DurationList(const DurationList& other) = default;
	DurationList(DurationList&& other) = default;
	DurationList(std::initializer_list<std::chrono::seconds> initializer);
	DurationList(const std::list<std::chrono::seconds>& durations);
	DurationList(const std::list<std::chrono::milliseconds>& durationMillis);
	virtual ~DurationList() = default;

	DurationList adjust(unsigned adjustment) const;

	std::string toString() const;
};

///////////////////////////////////////////////////////////////////////////////

OSTREAM_INSERT(DurationList);

