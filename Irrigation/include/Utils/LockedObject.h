#pragma once


template <typename T> class LockedObject {

	std::shared_ptr<std::lock_guard<std::mutex>> lockGuard;
	ProgramWithMutexPtr programWithMutex;

	LockedObject& operator= (const LockedObject&);

public:
	LockedObject(const T& value, std::mutex* mutex);
	LockedObject(const LockedObject& other);
	~LockedObject();

	T operator-> ();
	const T operator-> () const;
	T& operator* ();
	const T& operator* () const;
};
