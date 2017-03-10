#pragma once

#include <array>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <utility>

#include "Logic/StartTime.h"
#include "Utils/LockedObject.h"


class Program;


////////////////////////////////////////////////////////////////

class RunTimeContainer {

	typedef std::array<std::pair<IdType, unsigned>, ZONE_COUNT> ContainerType;

public:

	typedef unsigned value_type;
	typedef ContainerType::const_iterator const_iterator;

private:

	RunTimeContainer(const RunTimeContainer&);
	RunTimeContainer& operator= (const RunTimeContainer&);

	ContainerType container;

public:
	RunTimeContainer();
	virtual ~RunTimeContainer();

	const_iterator begin() const { return container.begin(); }
	const_iterator end() const { return container.end(); }
	value_type& operator[] (IdType id) { return at(id); }
	const value_type& operator[] (IdType id) const { return at(id); }
	value_type& at(IdType id);
	const value_type& at(IdType id) const;
	size_t size() const { return container.size(); }
};

////////////////////////////////////////////////////////////////

class StartTimeContainer {

	typedef std::list<std::pair<IdType, StartTime>> ContainerType;

public:

	typedef StartTime value_type;
	typedef ContainerType::const_iterator const_iterator;

private:

	StartTimeContainer(const StartTimeContainer&);
	StartTimeContainer& operator= (const StartTimeContainer&);

	ContainerType::iterator find(IdType id);
	void insert(IdType id, const value_type& value);

	ContainerType container;

public:
	StartTimeContainer();
	virtual ~StartTimeContainer();

	IdType insert(const value_type& newItem);
	void erase(IdType id);
	void modify(IdType, const value_type& newItem);

	const_iterator begin() const { return container.begin(); }
	const_iterator end() const { return container.end(); }
	const value_type& operator[] (IdType id) const { return at(id); }
	const value_type& at(IdType id) const;
	size_t size() const;
};

////////////////////////////////////////////////////////////////


typedef LockedObject<Program> LockedProgram;

class ProgramContainer {

	typedef std::list<std::pair<IdType, Program*>> ProgramContainerType;
	typedef std::map<IdType, std::mutex*> MutexContainerType;

public:

	typedef Program* value_type;
	typedef std::function<void(IdType, LockedProgram)> CallbackType;

private:

	// Disable copy constructor and operator
	ProgramContainer(const ProgramContainer&);
	ProgramContainer& operator= (const ProgramContainer&);

	ProgramContainerType::iterator findProgram(IdType id);
	MutexContainerType::iterator findMutex(IdType id);

	mutable std::mutex mutex;
	ProgramContainerType programContainer;
	MutexContainerType mutexContainer;

public:

	ProgramContainer();
	virtual ~ProgramContainer();

	IdType insert(const value_type& newItem);
	void erase(IdType id);
	void move(IdType id, size_t newPosition);

	void iterate(CallbackType f) const;
	const LockedProgram operator[] (IdType id) const { return at(id); }
	const LockedProgram at(IdType id) const;
	size_t size() const;
};
