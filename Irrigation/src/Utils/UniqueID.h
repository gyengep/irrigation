#pragma once

class UniqueID {
	static std::unique_ptr<UniqueID> instance;

	std::atomic<IdType> nextId;

	UniqueID();

public:

	static UniqueID& getInstance();

	~UniqueID();
	IdType getNextId();
};
