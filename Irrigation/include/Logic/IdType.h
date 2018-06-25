#pragma once
#include <string>


class IdType {
	static unsigned lastUsedId;
	const unsigned id;

public:
	IdType();
	IdType(unsigned id);

	IdType& operator= (unsigned value) { return *this; }
	operator unsigned() const { return id; }

	friend std::string to_string(const IdType& idType);
};
