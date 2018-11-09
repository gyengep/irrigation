#pragma once
#include <cstddef>
#include <list>
#include <memory>
#include "IdType.h"

class Program;


class ProgramContainer {
public:
	typedef IdType									key_type;
	typedef std::unique_ptr<Program>				mapped_type;
	typedef std::pair<const key_type, mapped_type>	value_type;
	typedef std::list<value_type>					container_type;
	typedef typename container_type::const_iterator const_iterator;

private:

	container_type container;

	container_type::const_iterator find(const key_type& key) const;

public:
	ProgramContainer() = default;
	ProgramContainer(ProgramContainer&&) = delete;
	ProgramContainer(const ProgramContainer&) = delete;
	virtual ~ProgramContainer() = default;

	ProgramContainer& operator= (ProgramContainer&&) = delete;
	ProgramContainer& operator= (const ProgramContainer&) = delete;

	virtual value_type& insert(const key_type& key, mapped_type&& value);
	virtual void erase(const key_type& key);

	const_iterator begin() const 		{ return container.begin(); }
	const_iterator end() const 			{ return container.end(); }
	size_t size() const 				{ return container.size(); }

	const mapped_type& at(const key_type& key) const;
};
