#pragma once
#include <list>
#include <memory>
#include "DTO/StartTimeDto.h"
#include "Utils/OstreamInsert.h"
#include "IdType.h"
#include "StartTime.h"


class StartTimeContainer {
public:
	typedef IdType									key_type;
	typedef StartTimePtr							mapped_type;
	typedef ConstStartTimePtr						const_mapped_type;
	typedef std::pair<const key_type, mapped_type>	value_type;
	typedef std::list<value_type>					container_type;
	typedef typename container_type::const_iterator const_iterator;
	typedef container_type::size_type				size_type;

	virtual ~StartTimeContainer() = default;

	virtual const_iterator begin() const = 0;
	virtual const_iterator end() const = 0;
	virtual size_t size() const = 0;
	virtual bool empty() const = 0;

	virtual value_type& insert(const key_type& key, const mapped_type& value) = 0;		// throw AlreadyExistException
	virtual void erase(const key_type& key) = 0;										// throw NoSuchElementException
	virtual void sort() = 0;

	virtual const_mapped_type at(const key_type& key) const = 0;						// throw NoSuchElementException
	virtual mapped_type at(const key_type& key) = 0;									// throw NoSuchElementException

	virtual std::list<StartTimeDto> toStartTimeDtoList() const = 0;
	virtual void updateFromStartTimeDtoList(const std::list<StartTimeDto>& startTimeDtoList) = 0;
	virtual value_type& createFromStartTimeDto(const StartTimeDto& startTimeDto) = 0;

	virtual std::string toString() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

OSTREAM_INSERT(StartTimeContainer);

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<StartTimeContainer> StartTimeContainerPtr;

///////////////////////////////////////////////////////////////////////////////

class StartTimeContainerFactory {
public:
	virtual ~StartTimeContainerFactory() = default;
	virtual StartTimeContainerPtr create() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<StartTimeContainerFactory> StartTimeContainerFactoryPtr;
