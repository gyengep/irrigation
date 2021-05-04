#include "SynchronizationObject.h"
#include "WaitObserverStore.h"


SynchronizationObject::SynchronizationObject() {
}

SynchronizationObject::~SynchronizationObject() {
}

void SynchronizationObject::setName(const std::string& name) {
	waitObserver = WaitObserverStore::getInstance().find(name);
}

void SynchronizationObject::wait_for(const std::chrono::milliseconds& ms) {
	std::unique_lock<std::mutex> lock(mtx, std::adopt_lock);

	if (lock.mutex()->try_lock()) {
		lock.mutex()->unlock();
		throw IllegalStateException("SynchronizationObject::wait(ms) !unique_lock.owns_lock()");
	}

	if (waitObserver) {
		waitObserver->wait_for(ms);
	}

	if (std::cv_status::no_timeout == cv.wait_for(lock, ms)) {
		throw InterruptedException("SynchronizationObject::wait_for(ms) is interrupted");
	}
}

void SynchronizationObject::wait() {
	std::unique_lock<std::mutex> lock(mtx, std::adopt_lock);

	if (lock.mutex()->try_lock()) {
		lock.mutex()->unlock();
		throw IllegalStateException("SynchronizationObject::wait() !unique_lock.owns_lock()");
	}

	if (waitObserver) {
		waitObserver->wait();
	}

	cv.wait(lock);
	throw InterruptedException("SynchronizationObject::wait() is interrupted");
}

void SynchronizationObject::notify_one() {
	cv.notify_one();
}

void SynchronizationObject::notify_all() {
	cv.notify_all();
}

std::unique_ptr<std::unique_lock<std::mutex>> SynchronizationObject::synchronize() {
	return std::unique_ptr<std::unique_lock<std::mutex>>(new std::unique_lock<std::mutex>(mtx));
}
