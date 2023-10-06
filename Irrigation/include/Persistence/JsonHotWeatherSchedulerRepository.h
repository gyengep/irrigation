#pragma once
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include "Utils/FileReader.h"
#include "Utils/FileWriter.h"
#include "HotWeatherSchedulerEntity.h"
#include "HotWeatherSchedulerRepository.h"


class JsonHotWeatherSchedulerRepository : public HotWeatherSchedulerRepository {
	mutable std::mutex mtx;

	const std::shared_ptr<FileReader> fileReader;
	const std::shared_ptr<FileWriterFactory> fileWriterFactory;

	std::map<unsigned, HotWeatherSchedulerEntityPtr> entities;

	void save_unsafe() const;

public:
	JsonHotWeatherSchedulerRepository(
			const std::shared_ptr<FileReader>& fileReader,
			const std::shared_ptr<FileWriterFactory>& fileWriterFactory
		);
	virtual ~JsonHotWeatherSchedulerRepository();

	virtual void init() override;
	virtual void uninit() override;

	virtual std::list<HotWeatherSchedulerEntityPtr> findAll() const override;
	virtual const HotWeatherSchedulerEntityPtr findById(unsigned id) const override;

	virtual void insert(const HotWeatherSchedulerEntityPtr& entity) override;
	virtual void erase(const HotWeatherSchedulerEntityPtr& entity) override;
	virtual void update(const HotWeatherSchedulerEntityPtr& entity) override;

	virtual std::string toString() const override;

	// for testing
	const std::map<unsigned, HotWeatherSchedulerEntityPtr>& getEntities() const { return entities; }
};
