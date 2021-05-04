#include "WaitObserverStore.h"


WaitObserverStore& WaitObserverStore::getInstance() {
	static WaitObserverStore instance;
	return instance;
}

///////////////////////////////////////////////////////////////////////////////

WaitObserverStore::WaitObserverStore() {
}

WaitObserverStore::~WaitObserverStore() {
}

void WaitObserverStore::clear() {
	waitObservers.clear();
}

void WaitObserverStore::insert(const std::string& name, const std::shared_ptr<WaitObserver>& waitObserver) {
	waitObservers.insert(std::make_pair(name, waitObserver));
}

std::shared_ptr<WaitObserver> WaitObserverStore::find(const std::string& name) const {
	auto it = waitObservers.find(name);

	if (waitObservers.end() == it) {
		return std::shared_ptr<WaitObserver>();
	}

	return it->second;
}
