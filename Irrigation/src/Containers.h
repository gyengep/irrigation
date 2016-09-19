/*
 * Containers.h
 *
 *  Created on: 2016.09.19.
 *      Author: 502664609
 */

#ifndef CONTAINERS_H_
#define CONTAINERS_H_

#include <array>
#include <list>
#include <mutex>

#include "common.h"


class Program;

////////////////////////////////////////////////////////////////

class RunTimeContainer {
public:
	typedef std::array<std::pair<IdType, unsigned>, ZONE_COUNT> RunTimes;

private:
	RunTimes runTimes;

public:
	RunTimeContainer();
	virtual ~RunTimeContainer();

	const RunTimes& container() const;
	void set(IdType id, unsigned minutes);
	unsigned get(IdType id) const;
};

////////////////////////////////////////////////////////////////

class StartTimeContainer {
public:
	typedef std::list<std::pair<IdType, unsigned>> StartTimes;

private:
	StartTimes startTimes;
	IdType nextStartTimeId;

public:
	StartTimeContainer();
	virtual ~StartTimeContainer();

	const StartTimes& container() const;
	IdType add(unsigned minutes);
	void del(IdType id);
	void set(IdType id, unsigned minutes);
	unsigned get(IdType id) const;
};

////////////////////////////////////////////////////////////////

class ProgramContainer {

public:
	typedef std::list<std::pair<IdType, Program*>> Programs;

private:
	mutable std::mutex mutex;
	IdType nextProgramId;
	Programs programs;

public:
	ProgramContainer();
	virtual ~ProgramContainer();

	std::mutex& getMutex() const { return mutex; }
	const Programs& container() const;
	Program& add();
	void del(IdType id);
	void move(IdType id, unsigned newPosition);
	Program& get(IdType id);
	const Program& get(IdType id) const;
};

#endif /* CONTAINERS_H_ */
