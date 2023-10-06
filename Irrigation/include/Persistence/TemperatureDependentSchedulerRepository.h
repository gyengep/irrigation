#pragma once
#include <list>
#include <memory>
#include "TemperatureDependentSchedulerEntity.h"


class TemperatureDependentSchedulerRepository {
public:
	virtual ~TemperatureDependentSchedulerRepository() = default;

	virtual void init() = 0;
	virtual void uninit() = 0;

	virtual std::list<TemperatureDependentSchedulerEntityPtr> findAll() const = 0;
	virtual const TemperatureDependentSchedulerEntityPtr findById(unsigned id) const = 0;

	virtual void insert(const TemperatureDependentSchedulerEntityPtr& entity) = 0;
	virtual void erase(const TemperatureDependentSchedulerEntityPtr& entity) = 0;
	virtual void update(const TemperatureDependentSchedulerEntityPtr& entity) = 0;

	virtual std::string toString() const = 0;
};
