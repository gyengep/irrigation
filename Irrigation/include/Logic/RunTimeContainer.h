#pragma once
#include <chrono>
#include <list>
#include <memory>
#include <string>
#include <vector>
#include "DTO/RunTimeDTO.h"
#include "Utils/OstreamInsert.h"
#include "DurationList.h"
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

	RunTimeContainer() = default;
	virtual ~RunTimeContainer() = default;

	virtual const_iterator begin() const = 0;
	virtual const_iterator end() const = 0;
	virtual size_t size() const = 0;

	virtual const_mapped_type at(const key_type& key) const = 0;
	virtual mapped_type at(const key_type& key) = 0;

	virtual std::list<RunTimeDTO> toRunTimeDtoList() const = 0;
	virtual void updateFromRunTimeDtoList(const std::list<RunTimeDTO>& runTimeDtoList) = 0;

	virtual std::string toString() const = 0;
	virtual DurationList toDurationList() const = 0;

	static DurationList toDurationList(const std::list<RunTimeDTO>& runTimeDtoList);
};

///////////////////////////////////////////////////////////////////////////////

OSTREAM_INSERT(RunTimeContainer);

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<RunTimeContainer> RunTimeContainerPtr;

///////////////////////////////////////////////////////////////////////////////

class RunTimeContainerFactory {
public:
	virtual ~RunTimeContainerFactory() = default;
	virtual RunTimeContainerPtr create() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<RunTimeContainerFactory> RunTimeContainerFactoryPtr;
