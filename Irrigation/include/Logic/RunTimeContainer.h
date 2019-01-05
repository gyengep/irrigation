#pragma once
#include <cstddef>
#include <memory>
#include <vector>
#include "IdType.h"

class RunTime;
class RunTimeFactory;


class RunTimeContainer {
public:
	typedef IdType									key_type;
	typedef std::shared_ptr<RunTime>				mapped_type;
	typedef std::pair<const key_type, mapped_type>	value_type;
	typedef std::vector<value_type>					container_type;
	typedef typename container_type::const_iterator const_iterator;
	typedef container_type::size_type				size_type;

private:

	container_type container;

public:
	RunTimeContainer();
	RunTimeContainer(RunTimeContainer&&) = delete;
	RunTimeContainer(const RunTimeContainer& other);
	RunTimeContainer(std::initializer_list<RunTime> initializer);
	virtual ~RunTimeContainer() = default;

	// for testing
	RunTimeContainer(std::unique_ptr<RunTimeFactory> runTimeFactory);

	RunTimeContainer& operator= (RunTimeContainer&&) = delete;
	RunTimeContainer& operator= (const RunTimeContainer& other);
	bool operator== (const RunTimeContainer& other) const;

	const_iterator begin() const 		{ return container.begin(); }
	const_iterator end() const 			{ return container.end(); }
	size_t size() const 				{ return container.size(); }

	const mapped_type& at(const key_type& key) const;

	friend std::string to_string(const RunTimeContainer& runTimeContainer);
};
