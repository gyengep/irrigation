#pragma once
#include <cstddef>
#include <list>
#include <memory>
#include "IdType.h"

class StartTime;


class StartTimeContainer {
public:
	typedef IdType									key_type;
	typedef std::unique_ptr<StartTime>				mapped_type;
	typedef std::pair<const key_type, mapped_type>	value_type;
	typedef std::list<value_type>					container_type;
	typedef typename container_type::const_iterator const_iterator;

private:

	container_type container;

	container_type::const_iterator find(const key_type& key) const;
	static bool compareStartTime(const value_type& first, const value_type& second);

public:
	StartTimeContainer() = default;
	StartTimeContainer(StartTimeContainer&&) = delete;
	StartTimeContainer(const StartTimeContainer&) = delete;
	virtual ~StartTimeContainer() = default;

	virtual value_type& insert(const key_type& key, mapped_type&& value);
	virtual void erase(const key_type& key);
	virtual void sort();

	const_iterator begin() const 		{ return container.begin(); }
	const_iterator end() const 			{ return container.end(); }
	size_t size() const 				{ return container.size(); }

	const mapped_type& at(const key_type& key) const;

	StartTimeContainer& operator= (StartTimeContainer&&) = delete;
	StartTimeContainer& operator= (const StartTimeContainer&) = delete;

	friend std::string to_string(const StartTimeContainer& startTimeContainer);
};
