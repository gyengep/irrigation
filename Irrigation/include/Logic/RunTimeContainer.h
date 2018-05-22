#pragma once
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>
#include "IdType.h"

class RunTime;


class RunTimeFactory {
public:
	virtual ~RunTimeFactory() {}
	virtual RunTime* createRunTime() const;
};


class RunTimeContainer {
public:
	typedef IdType									key_type;
	typedef RunTime*								mapped_type;
	typedef std::pair<const key_type, mapped_type>	value_type;
	typedef std::vector<value_type>					container_type;
	typedef typename container_type::const_iterator const_iterator;

private:

	// Disable copy constructor and operator
	RunTimeContainer(const RunTimeContainer&);
	RunTimeContainer& operator= (const RunTimeContainer&);

	void init(std::unique_ptr<const RunTimeFactory> runTimeFactory);

	container_type container;

public:
	RunTimeContainer();
	virtual ~RunTimeContainer();

	// for testing
	RunTimeContainer(const RunTimeFactory* runTimeFactory);

	const_iterator begin() const 		{ return container.begin(); }
	const_iterator end() const 			{ return container.end(); }
	size_t size() const 				{ return container.size(); }

	const mapped_type& at(const key_type& key) const;
	mapped_type& at(const key_type& key);
};
