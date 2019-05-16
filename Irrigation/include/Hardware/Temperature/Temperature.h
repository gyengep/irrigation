#pragma once
#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include "Utils/Timer.h"
#include "TemperatureException.h"

class FileReader;


class Temperature : public TimerCallback {
	static const std::string basePath;
	static const std::string fileName;
	static std::shared_ptr<Temperature> instance;

	std::unique_ptr<Timer> timer;
	std::mutex mtx;
	std::shared_ptr<FileReader> fileReader;
	bool valid;
	float value;

	static std::string getTempSensorFileName();

public:
	Temperature(const std::shared_ptr<FileReader>& fileReader);
	virtual ~Temperature();

	void lock();
	void unlock();

	void refresh();
	float readValueFromSensor();

	bool isValid() const;
	float getCachedValue() const;

	void startPeriodicRefresh();
	void stopPeriodicRefresh();
	virtual void onTimer() override;

	static void init();
	static std::shared_ptr<Temperature> getInstancePtr();
};
