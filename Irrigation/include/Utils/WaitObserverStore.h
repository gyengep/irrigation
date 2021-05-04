#pragma once
#include <map>
#include <memory>
#include "WaitObserver.h"


class WaitObserverStore {
	std::map<std::string, std::shared_ptr<WaitObserver>> waitObservers;

	WaitObserverStore();

public:
	virtual ~WaitObserverStore();

	void clear();
	void insert(const std::string& name, const std::shared_ptr<WaitObserver>& waitObserver);
	std::shared_ptr<WaitObserver> find(const std::string& name) const;

	static WaitObserverStore& getInstance();
};
