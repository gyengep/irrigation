/*
 * Containers.h
 *
 *  Created on: 2016.09.19.
 *      Author: 502664609
 */

#ifndef CONTAINERS_H_
#define CONTAINERS_H_

#include <array>

#include "common.h"


class RunTimeContainer_set_Test;

class Containers {
public:
	Containers();
	virtual ~Containers();
};

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


#endif /* CONTAINERS_H_ */
