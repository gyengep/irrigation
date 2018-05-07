#pragma once
#include <list>
#include <cstddef>
#include <stdexcept>
#include <utility>
#include "StartTime.h"


typedef unsigned IdType;



class InvalidStartTimeIdException : public std::runtime_error {
public:
	explicit InvalidStartTimeIdException(IdType id) : std::runtime_error("Invalid starttimeID" + std::to_string(id)) {}
};



class StartTimeContainer {
public:
	typedef IdType									key_type;
	typedef StartTime*								mapped_type;
	typedef std::pair<const key_type, mapped_type>	value_type;
	typedef std::list<value_type>					ContainerType;
	typedef typename ContainerType::const_iterator 	const_iterator;

private:

	// Disable copy constructor and operator
	StartTimeContainer(const StartTimeContainer&);
	StartTimeContainer& operator= (const StartTimeContainer&);

	ContainerType::const_iterator find(const key_type& key) const;
	ContainerType::iterator find(const key_type& key);

	static bool compareStartTime(const value_type& first, const value_type& second);

	ContainerType container;

public:
	StartTimeContainer();
	virtual ~StartTimeContainer();

	virtual void insert(const value_type& newItem);
	virtual void erase(key_type key);
	virtual void sort();

	const_iterator begin() const 		{ return container.begin(); }
	const_iterator end() const 			{ return container.end(); }
	size_t size() const 				{ return container.size(); }

	const mapped_type& at(const key_type& key) const;
	mapped_type& at(const key_type& key);
};
