#pragma once
#include <string>
#include "Utils/OstreamInsert.h"


class IdType {
	static unsigned lastUsedId;
	unsigned id;

public:
	IdType();
	IdType(IdType&&) = default;
	IdType(const IdType&) = default;
	IdType(unsigned id);

	IdType& operator= (IdType&&) = default;
	IdType& operator= (const IdType&) = default;
	bool operator== (const IdType& other) const;

	unsigned getValue() const;
	std::string toString() const;

	static IdType from_string(const std::string& id); // throws IllegalArgumentException
};

///////////////////////////////////////////////////////////////////////////////

OSTREAM_INSERT(IdType);
