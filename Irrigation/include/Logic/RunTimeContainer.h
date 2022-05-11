#pragma once
#include <cstddef>
#include <list>
#include <memory>
#include <vector>
#include "IdType.h"
#include "RunTime.h"
#include "DTO/RunTimeDTO.h"


class RunTimeContainer {
public:
	typedef IdType									key_type;
	typedef RunTimePtr								mapped_type;
	typedef std::pair<const key_type, mapped_type>	value_type;
	typedef std::vector<value_type>					container_type;
	typedef typename container_type::const_iterator const_iterator;
	typedef container_type::size_type				size_type;

private:

	container_type container;

public:
	RunTimeContainer(const std::shared_ptr<RunTimeFactory>& runTimeFactory);
	RunTimeContainer(RunTimeContainer&&) = default;
	RunTimeContainer(const RunTimeContainer& other) = delete;
	RunTimeContainer(std::initializer_list<RunTimePtr> initializer);
	virtual ~RunTimeContainer() = default;

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
