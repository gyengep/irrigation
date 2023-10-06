#pragma once
#include <list>
#include <memory>
#include <string>
#include "HotWeatherSchedulerEntity.h"


class HotWeatherSchedulerRepository {
public:
	virtual ~HotWeatherSchedulerRepository() = default;

	virtual void init() = 0;
	virtual void uninit() = 0;

	virtual std::list<HotWeatherSchedulerEntityPtr> findAll() const = 0;
	virtual const HotWeatherSchedulerEntityPtr findById(unsigned id) const = 0;

	virtual void insert(const HotWeatherSchedulerEntityPtr& entity) = 0;
	virtual void erase(const HotWeatherSchedulerEntityPtr& entity) = 0;
	virtual void update(const HotWeatherSchedulerEntityPtr& entity) = 0;

	virtual std::string toString() const = 0;
};
