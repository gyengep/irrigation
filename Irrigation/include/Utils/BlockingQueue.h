#pragma once
#include <condition_variable>
#include <mutex>
#include <queue>


template <typename T>
class BlockingQueue {
	mutable std::mutex mtx;
	mutable std::condition_variable cv;
	bool interrupted;

	std::queue<T> container;

public:

	BlockingQueue();
	~BlockingQueue() = default;

	void push(const T& value);
	void push(T&& value);
	void pop();
	const T& front() const;

	size_t size() const;

	bool waitForElement() const;
	void interrupt();
};


template <typename T>
BlockingQueue<T>::BlockingQueue() :
	interrupted(false)
{
}

template <typename T>
size_t BlockingQueue<T>::size() const {
	std::unique_lock<std::mutex> lock(mtx);
	return container.size();
}

template <typename T>
void BlockingQueue<T>::push(const T& value) {
	std::unique_lock<std::mutex> lock(mtx);

	if (interrupted) {
		throw std::logic_error("BlockingQueue<T>::push(const T& value) finished == true");
	}

	container.push(value);
	cv.notify_one();
}

template <typename T>
void BlockingQueue<T>::push(T&& value) {
	std::unique_lock<std::mutex> lock(mtx);

	if (interrupted) {
		throw std::logic_error("BlockingQueue<T>::push(T&& value) finished == true");
	}

	container.push(std::move(value));
	cv.notify_one();
}

template <typename T>
const T& BlockingQueue<T>::front() const {
	std::unique_lock<std::mutex> lock(mtx);

	if (container.empty()) {
		throw std::logic_error("BlockingQueue<T>::front() container.empty()");
	}

	return container.front();
}

template <typename T>
void BlockingQueue<T>::pop() {
	std::unique_lock<std::mutex> lock(mtx);

	if (container.empty()) {
		throw std::logic_error("BlockingQueue<T>::pop() container.empty()");
	}

	container.pop();
}

template <typename T>
bool BlockingQueue<T>::waitForElement() const {
	std::unique_lock<std::mutex> lock(mtx);

	cv.wait(lock, [this] {
		return (!container.empty() || interrupted);
	});

	return (false ==  container.empty());
}

template <typename T>
void BlockingQueue<T>::interrupt() {
	std::unique_lock<std::mutex> lock(mtx);
	interrupted = true;
	cv.notify_one();
}
