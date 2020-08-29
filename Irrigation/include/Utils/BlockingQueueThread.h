#pragma once
#include <ctime>
#include <memory>
#include "Exceptions/InterruptedException.h"
#include "BlockingQueue.h"
#include "IncrementalWait.h"
#include "TimeConversion.h"
#include "Thread.h"


template <typename T>
class BlockingQueueThread : public Thread {
	BlockingQueue<T> queue;
	const std::unique_ptr<IncrementalWait> wait;

	virtual void onExecute() override;
	virtual void onItemAvailable(const T& value) = 0;

protected:
	std::chrono::milliseconds getWaitTime() const;

public:
	BlockingQueueThread(const std::string& name = "");
	BlockingQueueThread(const std::string& name, const std::vector<std::chrono::milliseconds>& waitTimes);
	virtual ~BlockingQueueThread() = default;

	void push(const T& value);
	void push(T&& value);

	void stop();
};

template <typename T>
BlockingQueueThread<T>::BlockingQueueThread(const std::string& name) : Thread(name),
	wait()
{
}

template <typename T>
BlockingQueueThread<T>::BlockingQueueThread(const std::string& name, const std::vector<std::chrono::milliseconds>& waitTimes) :
	Thread(name),
	wait(new IncrementalWait(waitTimes))
{
}

template <typename T>
std::chrono::milliseconds BlockingQueueThread<T>::getWaitTime() const {
	if (!wait) {
		return std::chrono::milliseconds(0);
	}

	return wait->getWaitTime();
}

template <typename T>
void BlockingQueueThread<T>::push(const T& value) {
	queue.push(value);
}

template <typename T>
void BlockingQueueThread<T>::push(T&& value) {
	queue.push(std::move(value));
}

template <typename T>
void BlockingQueueThread<T>::stop() {
	queue.finish();

	if (wait) {
		wait->interrupt();
	}

	join();
}

template <typename T>
void BlockingQueueThread<T>::onExecute() {
	try {

		while (queue.waitForElement()) {
			try {
				onItemAvailable(queue.front());
				queue.pop();

				if (wait) {
					wait->resetWaitTime();
				}
			} catch (const std::exception& e) {
				if (wait) {
					wait->wait();
					wait->incrementWaitTime();
				}
			}
		}

	} catch (const InterruptedException& e) {
	}
}
