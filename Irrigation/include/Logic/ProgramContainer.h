#pragma once
#include <list>
#include <memory>
#include <mutex>
//#include <utility>
#include "IdType.h"


class Program;



class LockedProgram;

class ProgramContainer {
public:

	struct ProgramWithMutex {
		std::unique_ptr<Program> program;
		std::unique_ptr<std::mutex> mutex;

		ProgramWithMutex(Program* program, std::mutex* mutex);
	};

	typedef std::shared_ptr<ProgramWithMutex> ProgramWithMutexPtr;
	typedef std::list<std::pair<IdType, ProgramWithMutexPtr>> ProgramContainerType;
	typedef Program* value_type;
	typedef std::function<void(IdType, LockedProgram)> CallbackType;

protected:

	// Accessible only for ContainerFactory and ProgramContainerMock
	ProgramContainer();

private:
	// Disable copy constructor and operator
	ProgramContainer(const ProgramContainer&);
	ProgramContainer& operator= (const ProgramContainer&);

	ProgramContainerType::iterator find(IdType id);

	mutable std::mutex mutex;
	ProgramContainerType programContainer;

public:

	virtual ~ProgramContainer();

	virtual IdType insert(const value_type& newItem);
	virtual void erase(IdType id);
	virtual void move(IdType id, size_t newPosition);

	void iterate(CallbackType f) const;
	const LockedProgram at(IdType id) const;
	LockedProgram at(IdType id);
	size_t size() const;
};


class LockedProgram {

	std::shared_ptr<std::lock_guard<std::mutex>> lockGuard;
	ProgramContainer::ProgramWithMutexPtr programWithMutex;

	LockedProgram& operator= (const LockedProgram&);

public:
	LockedProgram(ProgramContainer::ProgramWithMutexPtr programWithMutex);
	LockedProgram(const LockedProgram& other);
	~LockedProgram();

	Program* operator-> ();
	const Program* operator-> () const;
	Program& operator* ();
	const Program& operator* () const;
};

