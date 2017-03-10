#pragma once


template <typename TYPE> class LockedObject {
	// Disable copy operator
	LockedObject& operator= (const LockedObject&);

	struct Holder {
		TYPE* ptr;
		std::mutex* mutex;

		Holder(TYPE* ptr, std::mutex* mutex) : ptr(ptr), mutex(mutex) {
			mutex->lock();
		}
		~Holder() {
			mutex->unlock();
		}
	};

	std::shared_ptr<Holder> holder;

public:

	LockedObject(TYPE* ptr, std::mutex* mutex) : holder(new Holder(ptr, mutex)){
	}

	LockedObject(const LockedObject& other) : holder(other.holder) {
	}

	virtual ~LockedObject() {
	}

	TYPE* operator-> () {
		return holder->ptr;
	}

	const TYPE* operator-> () const {
		return holder->ptr;
	}

	TYPE& operator* () {
		return *(holder->ptr);
	}

	const TYPE& operator* () const {
		return *(holder->ptr);
	}
};
