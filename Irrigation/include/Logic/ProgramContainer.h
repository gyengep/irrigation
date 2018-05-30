#pragma once
#include <cstddef>
#include <list>
#include "IdType.h"

class Program;


class ProgramContainer {
public:
	typedef IdType									key_type;
	typedef Program*								mapped_type;
	typedef std::pair<const key_type, mapped_type>	value_type;
	typedef std::list<value_type>					container_type;
	typedef typename container_type::const_iterator const_iterator;

private:

	// Disable copy constructor and operator
	ProgramContainer(const ProgramContainer&);
	ProgramContainer& operator= (const ProgramContainer&);

	container_type::iterator find(const key_type& key);
	container_type::const_iterator find(const key_type& key) const;

	container_type container;

public:
	ProgramContainer();
	virtual ~ProgramContainer();

	virtual void insert(const key_type& key, const mapped_type& value);
	virtual void erase(const key_type& key);

	const_iterator begin() const 		{ return container.begin(); }
	const_iterator end() const 			{ return container.end(); }
	size_t size() const 				{ return container.size(); }

	const mapped_type& at(const key_type& key) const;
	mapped_type& at(const key_type& key);
};
