#pragma once
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>
#include "RunTime.h"
#include "ZoneConfig.h"


typedef unsigned IdType;


class InvalidRunTimeIdException : public std::runtime_error {
public:
	explicit InvalidRunTimeIdException(IdType id) : std::runtime_error("Invalid runtimeID" + std::to_string(id)) {}
};



class RunTimeFactory {
public:
	virtual ~RunTimeFactory() {}
	virtual RunTime* createRunTime() {
		return new RunTime();
	}
};



class RunTimeContainer {
public:
	typedef IdType									key_type;
	typedef RunTime*								mapped_type;
	typedef std::pair<const key_type, mapped_type>	value_type;
	typedef std::vector<value_type>					ContainerType;
	typedef typename ContainerType::const_iterator 	const_iterator;

private:

	// Disable copy constructor and operator
	RunTimeContainer(const RunTimeContainer&);
	RunTimeContainer& operator= (const RunTimeContainer&);

	void init(RunTimeFactory* runTimeFactory);

	ContainerType container;

public:
	RunTimeContainer();
	RunTimeContainer(RunTimeFactory* runTimeFactory);
	virtual ~RunTimeContainer();

	const_iterator begin() const 		{ return container.begin(); }
	const_iterator end() const 			{ return container.end(); }
	size_t size() const 				{ return container.size(); }

	const mapped_type& at(const key_type& key) const;
	mapped_type& at(const key_type& key);
};
