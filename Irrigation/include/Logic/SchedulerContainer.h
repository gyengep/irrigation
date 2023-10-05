#pragma once
#include <map>
#include <memory>
#include <string>
#include "DTO/SchedulersDTO.h"
#include "Utils/OstreamInsert.h"
#include "Schedulers/Scheduler.h"
#include "Schedulers/EveryDayScheduler.h"
#include "Schedulers/HotWeatherScheduler.h"
#include "Schedulers/TemperatureDependentScheduler.h"
#include "Schedulers/WeeklyScheduler.h"


class SchedulerContainer {
public:

	typedef SchedulerType							key_type;
	typedef SchedulerPtr							mapped_type;
	typedef ConstSchedulerPtr						const_mapped_type;
	typedef std::pair<const key_type, mapped_type>  value_type;
	typedef std::map<key_type, mapped_type>			container_type;
	typedef typename container_type::const_iterator const_iterator;
	typedef container_type::size_type				size_type;

	virtual ~SchedulerContainer() = default;

	virtual const_iterator begin() const = 0;
	virtual const_iterator end() const = 0;
	virtual size_t size() const = 0;

	virtual const_mapped_type at(const key_type& key) const = 0;
	virtual mapped_type at(const key_type& key) = 0;

	virtual const EveryDayScheduler& getEveryDayScheduler() const = 0;
	virtual const HotWeatherScheduler& getHotWeatherScheduler() const = 0;
	virtual const TemperatureDependentScheduler& getTemperatureDependentScheduler() const = 0;
	virtual const WeeklyScheduler& getWeeklyScheduler() const = 0;

	virtual EveryDayScheduler& getEveryDayScheduler() = 0;
	virtual HotWeatherScheduler& getHotWeatherScheduler() = 0;
	virtual TemperatureDependentScheduler& getTemperatureDependentScheduler() = 0;
	virtual WeeklyScheduler& getWeeklyScheduler() = 0;

	virtual SchedulersDTO toSchedulersDto() const = 0;
	virtual void updateFromSchedulersDto(const SchedulersDTO& schedulersDto) = 0;

	virtual std::string toString() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

OSTREAM_INSERT(SchedulerContainer);

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<SchedulerContainer> SchedulerContainerPtr;

///////////////////////////////////////////////////////////////////////////////

class SchedulerContainerFactory {
public:
	virtual ~SchedulerContainerFactory() = default;
	virtual SchedulerContainerPtr create() const = 0;
};

///////////////////////////////////////////////////////////////////////////////

typedef std::shared_ptr<SchedulerContainerFactory> SchedulerContainerFactoryPtr;
