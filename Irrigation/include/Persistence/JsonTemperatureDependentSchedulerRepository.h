#pragma once
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include "Utils/FileReader.h"
#include "Utils/FileWriter.h"
#include "TemperatureDependentSchedulerEntity.h"
#include "TemperatureDependentSchedulerRepository.h"


class JsonTemperatureDependentSchedulerRepository : public TemperatureDependentSchedulerRepository {
	mutable std::mutex mtx;

	const std::shared_ptr<FileReader> fileReader;
	const std::shared_ptr<FileWriterFactory> fileWriterFactory;

	std::map<unsigned, TemperatureDependentSchedulerEntityPtr> entities;

	void save_unsafe() const;

public:
	JsonTemperatureDependentSchedulerRepository(
			const std::shared_ptr<FileReader>& fileReader,
			const std::shared_ptr<FileWriterFactory>& fileWriterFactory
		);
	virtual ~JsonTemperatureDependentSchedulerRepository();

	virtual void init() override;
	virtual void uninit() override;

	virtual std::list<TemperatureDependentSchedulerEntityPtr> findAll() const override;
	virtual const TemperatureDependentSchedulerEntityPtr findById(unsigned id) const override;

	virtual void insert(const TemperatureDependentSchedulerEntityPtr& entity) override;
	virtual void erase(const TemperatureDependentSchedulerEntityPtr& entity) override;
	virtual void update(const TemperatureDependentSchedulerEntityPtr& entity) override;

	virtual std::string toString() const override;

	// for testing
	const std::map<unsigned, TemperatureDependentSchedulerEntityPtr>& getEntities() const { return entities; }
};
