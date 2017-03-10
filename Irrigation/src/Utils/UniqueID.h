#pragma once

class UniqueID {
	static UniqueID* instance;

	IdType nextId;

	UniqueID();

public:

	static UniqueID& getInstance();

	~UniqueID();
	IdType getNextId();
};
