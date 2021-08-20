#pragma once
#include <ctime>
#include <list>
#include <memory>
#include <mutex>
#include "Utils/CsvReader.h"
#include "Utils/CsvWriter.h"
#include "TemperatureHistoryRepository.h"


class CsvTemperatureHistoryRepository : public TemperatureHistoryRepository {
	const std::shared_ptr<CsvReaderFactory> csvReaderFactory;
	const std::shared_ptr<CsvWriterFactory> csvWriterFactory;

	mutable std::mutex mtx;
	std::list<Sample> samples;

	void add_withoutLock(const Sample& sample);

	static std::string temperatureToString(float value);

public:
	CsvTemperatureHistoryRepository(
			const std::shared_ptr<CsvReaderFactory>& csvReaderFactory,
			const std::shared_ptr<CsvWriterFactory>& csvWriterFactory
		);
	virtual ~CsvTemperatureHistoryRepository();

	void load();
	void save();

	virtual void removeOlder(const DateTime& dateTime) override;
	virtual void removeNewer(const DateTime& dateTime) override;
	virtual void add(const Sample& sample) override;
	virtual std::list<Sample> getBetween(const DateTime& from, const DateTime& to) const override;
	virtual std::list<Sample> getAll() const override;
};
