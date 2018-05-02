#pragma once

#include <array>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <utility>

#include "Logic/StartTime.h"
#include "Utils/Factory.h"


class Program;
class RunTimeContainerFactory;
class StartTimeContainerFactory;
class ProgramContainerFactory;


////////////////////////////////////////////////////////////////

class RunTimeContainer {
	friend RunTimeContainerFactory;

	typedef std::array<std::pair<IdType, unsigned>, ZONE_COUNT> ContainerType;

public:

	typedef unsigned value_type;
	typedef ContainerType::const_iterator const_iterator;

private:

	// Disable copy constructor and operator
	RunTimeContainer(const RunTimeContainer&);
	RunTimeContainer& operator= (const RunTimeContainer&);

	ContainerType container;

protected:

	// Accessible only for ContainerFactory and RunTimeContainerMock
	RunTimeContainer();

public:
	virtual ~RunTimeContainer();

	virtual void modify(IdType, const value_type& newItem);

	const_iterator begin() const { return container.begin(); }
	const_iterator end() const { return container.end(); }
	const value_type& at(IdType id) const;
	size_t size() const { return container.size(); }
};


////////////////////////////////////////////////////////////////


class StartTimeContainer {
	friend StartTimeContainerFactory;

	typedef std::list<std::pair<IdType, StartTime>> ContainerType;

public:

	typedef StartTime value_type;
	typedef ContainerType::const_iterator const_iterator;

private:

	// Disable copy constructor and operator
	StartTimeContainer(const StartTimeContainer&);
	StartTimeContainer& operator= (const StartTimeContainer&);

	ContainerType::iterator find(IdType id);
	void insert(IdType id, const value_type& value);

	ContainerType container;

protected:

	// Accessible only for ContainerFactory and StartTimeContainerMock
	StartTimeContainer();

public:
	virtual ~StartTimeContainer();

	virtual IdType insert(const value_type& newItem);
	virtual void erase(IdType id);
	virtual void modify(IdType, const value_type& newItem);

	const_iterator begin() const { return container.begin(); }
	const_iterator end() const { return container.end(); }
	const value_type& at(IdType id) const;
	size_t size() const;
};


////////////////////////////////////////////////////////////////

class LockedProgram;

class ProgramContainer {
	friend ProgramContainerFactory;
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

