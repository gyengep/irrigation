#pragma once
#include <cstddef>
#include <list>
#include <memory>
#include <string>
#include <vector>
#include "DTO/RunTimeDTO.h"
#include "Utils/OstreamInsert.h"
#include "IdType.h"
#include "RunTime.h"


class RunTimeContainer {
public:
	typedef IdType									key_type;
	typedef RunTimePtr								mapped_type;
	typedef ConstRunTimePtr							const_mapped_type;
	typedef std::pair<const key_type, mapped_type>	value_type;
	typedef std::vector<value_type>					container_type;
	typedef typename container_type::const_iterator const_iterator;
	typedef container_type::size_type				size_type;

private:
	container_type container;

public:
	RunTimeContainer(const std::shared_ptr<RunTimeFactory>& runTimeFactory);
	RunTimeContainer(RunTimeContainer&&) = delete;
	RunTimeContainer(const RunTimeContainer& other) = delete;
	RunTimeContainer(std::initializer_list<RunTimePtr> initializer);
	virtual ~RunTimeContainer() = default;

	RunTimeContainer& operator= (RunTimeContainer&&) = delete;
	RunTimeContainer& operator= (const RunTimeContainer& other) = delete;
	bool operator== (const RunTimeContainer& other) const;

	const_iterator begin() const 		{ return container.begin(); }
	const_iterator end() const 			{ return container.end(); }
	size_t size() const 				{ return container.size(); }

	const_mapped_type at(const key_type& key) const;
	mapped_type at(const key_type& key);

	std::list<RunTimeDTO> toRunTimeDtoList() const;
	virtual void updateFromRunTimeDtoList(const std::list<RunTimeDTO>& runTimeDtoList);

	std::string toString() const;
};

///////////////////////////////////////////////////////////////////////////////

OSTREAM_INSERT(RunTimeContainer);
