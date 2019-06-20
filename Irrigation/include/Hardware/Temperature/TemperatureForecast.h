#pragma once
#include <chrono>
#include <list>
#include <memory>
#include <mutex>
#include <string>
#include "Utils/Timer.h"
#include "TemperatureValues.h"


class TemperatureForecast : public TimerCallback {
public:
	class NetworkReader;

	struct Values {
		const float min;
		const float max;

		Values(float min, float max);

		// for testing
		bool operator== (const Values& other) const;
		friend std::ostream& operator<<(std::ostream& os, const Values& values);
	};

	struct ValuesWithTimes {
		const std::time_t from;
		const std::time_t to;
		const Values values;

		ValuesWithTimes(std::time_t from, std::time_t to, const Values& values);

		// for testing
		bool operator== (const ValuesWithTimes& other) const;
		friend std::ostream& operator<<(std::ostream& os, const ValuesWithTimes& valuesWithTimes);
	};

private:
	const std::string url;
	const std::string location;
	const std::string appid;

	mutable std::mutex mtx;
	std::list<ValuesWithTimes> temperatures;
	std::shared_ptr<NetworkReader> networkReader;
	Timer timer;

	static size_t writeCallback(char* buffer, size_t size, size_t nmemb, void* ctxt);
	static void addTemperature(std::list<ValuesWithTimes>& temperatures, std::time_t from, std::time_t to, const Values& values);

public:
	TemperatureForecast();
	TemperatureForecast(const std::shared_ptr<NetworkReader>& networkReader);
	virtual ~TemperatureForecast();

	void updateCache();

	void addTemperature(std::time_t from, std::time_t to, const Values& values);
	Values getForecast(std::time_t from, std::time_t to) const;

	void startTimer();
	void stopTimer();
	virtual void onTimer() override;

	// only for testing
	const std::list<ValuesWithTimes> getContainer() const;

	static std::time_t parseTimeString(const std::string& text);
};

class TemperatureForecast::NetworkReader {
public:
	virtual ~NetworkReader() = default;
	virtual std::string read(const std::string url, const std::string location, const std::string appid) const;
};
