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

#include "common.h"


class RunTimeContainer_set_Test;
class StartTimeContainer_add_Test;
class StartTimeContainer_del_Test;
class StartTimeContainer_set_Test;
class StartTimeContainer_get_Test;


class RunTimeContainer {
	friend RunTimeContainer_set_Test;

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
	friend StartTimeContainer_add_Test;
	friend StartTimeContainer_del_Test;
	friend StartTimeContainer_set_Test;
	friend StartTimeContainer_get_Test;

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


#endif /* CONTAINERS_H_ */
