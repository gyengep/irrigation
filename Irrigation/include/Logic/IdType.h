#pragma once
#include <string>


class IdType {
	static unsigned lastUsedId;
	unsigned id;

public:
	IdType();
	IdType(unsigned id);

	IdType& operator= (const IdType& other);
	operator unsigned() const { return id; }

	friend std::string to_string(const IdType& idType);
};
