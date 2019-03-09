#pragma once
#include <string>


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
	operator unsigned() const { return id; }

	friend std::string to_string(const IdType& idType);
	static IdType from_string(const std::string& id);
};
