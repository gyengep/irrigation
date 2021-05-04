#pragma once
#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include "Exceptions/IllegalStateException.h"
#include "Exceptions/InterruptedException.h"
#include "WaitObserver.h"


class SynchronizationObject {
	std::shared_ptr<WaitObserver> waitObserver;

	std::mutex mtx;
	std::condition_variable cv;

public:
	SynchronizationObject();
	~SynchronizationObject();

	void setName(const std::string& name);

	void wait_for(const std::chrono::milliseconds& ms);
	void wait();
	void notify_one();
	void notify_all();

	template<class Predicate> void wait_for(const std::chrono::milliseconds& ms, Predicate pred);
	template<class Predicate> void wait(Predicate pred);

	std::unique_ptr<std::unique_lock<std::mutex>> synchronize();
};

template<class Predicate>
void SynchronizationObject::wait_for(const std::chrono::milliseconds& ms, Predicate pred) {
	std::unique_lock<std::mutex> lock(mtx, std::adopt_lock);

	if (lock.mutex()->try_lock()) {
		lock.mutex()->unlock();
		throw IllegalStateException("SynchronizationObject::wait(ms, pred) !unique_lock.owns_lock()");
	}

	if (waitObserver) {
		waitObserver->wait_for_pred(ms);
	}

	if (cv.wait_for(lock, ms, pred)) {
		throw InterruptedException("SynchronizationObject::wait_for(ms) is interrupted");
	}
}

template<class Predicate>
void SynchronizationObject::wait(Predicate pred) {
	std::unique_lock<std::mutex> lock(mtx, std::adopt_lock);

	if (lock.mutex()->try_lock()) {
		lock.mutex()->unlock();
		throw IllegalStateException("SynchronizationObject::wait(pred) !unique_lock.owns_lock()");
	}

	if (waitObserver) {
		waitObserver->wait_pred();
	}

	cv.wait(lock, pred);
	throw InterruptedException("SynchronizationObject::wait() is interrupted");
}
