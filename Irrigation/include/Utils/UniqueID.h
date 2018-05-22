#pragma once
#include <atomic>
#include <memory>
#include <mutex>


class UniqueID {
	static std::mutex createMutex;
	static std::unique_ptr<UniqueID> instance;

	std::atomic<IdType> nextId;

	UniqueID();

public:

	static UniqueID& getInstance();

	~UniqueID();
	IdType getNextId();
};
