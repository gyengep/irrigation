#pragma once
#include <cstddef>
#include <list>
#include <memory>
#include <vector>
#include "IdType.h"
#include "DTO/RunTimeDTO.h"

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
	RunTimeContainer(RunTimeContainer&&) = default;
	RunTimeContainer(const RunTimeContainer& other);
	RunTimeContainer(std::initializer_list<RunTime> initializer);
	RunTimeContainer(const std::list<RunTimeDTO>& runTimeDtoList);
	virtual ~RunTimeContainer() = default;

	// for testing
	RunTimeContainer(std::shared_ptr<RunTimeFactory> runTimeFactory);

	RunTimeContainer& operator= (RunTimeContainer&&) = delete;
	RunTimeContainer& operator= (const RunTimeContainer& other) = delete;
	bool operator== (const RunTimeContainer& other) const;

	const_iterator begin() const 		{ return container.begin(); }
	const_iterator end() const 			{ return container.end(); }
	size_t size() const 				{ return container.size(); }

	const mapped_type& at(const key_type& key) const;

	std::list<RunTimeDTO> toRunTimeDtoList() const;
	virtual void updateFromRunTimeDtoList(const std::list<RunTimeDTO>& runTimeDtoList);

	friend std::string to_string(const RunTimeContainer& runTimeContainer);
	friend std::ostream& operator<<(std::ostream& os, const RunTimeContainer& runTimeContainer);
};

///////////////////////////////////////////////////////////////////////////////

class RunTimeFactory {
public:
	virtual ~RunTimeFactory() = default;
	virtual std::shared_ptr<RunTime> createRunTime();
};
