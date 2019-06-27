#pragma once
#include <chrono>
#include <list>
#include <memory>
#include <mutex>
#include <string>
#include "Utils/Timer.h"

#ifndef FRIEND_TEST
#define FRIEND_TEST(a, b)
#endif


class TemperatureForecast : public TimerCallback {
	FRIEND_TEST(TemperatureForecastTest, addTemperature);
	FRIEND_TEST(TemperatureForecastTest, addTemperatureInvalid);
	FRIEND_TEST(TemperatureForecastTest, addTemperatureMismatch);
	FRIEND_TEST(TemperatureForecastTest, getForecast);
	FRIEND_TEST(TemperatureForecastTest, getForecastValues);
	FRIEND_TEST(TemperatureForecastTest, getForecastOutOfBounds);

public:
	class  NetworkReader;
	struct Values;
	struct ValuesWithTimes;

private:
	static const std::string url;
	static const std::string location;
	static const std::string appid;

	mutable std::mutex mtx;
	std::list<ValuesWithTimes> temperatures;
	std::shared_ptr<NetworkReader> networkReader;
	std::unique_ptr<Timer> timer;

	void addTemperature(std::time_t from, std::time_t to, const Values& values);

	static size_t writeCallback(char* buffer, size_t size, size_t nmemb, void* ctxt);
	static void addTemperatureTo(std::list<ValuesWithTimes>& temperatures, std::time_t from, std::time_t to, const Values& values);

public:
	TemperatureForecast(const std::shared_ptr<NetworkReader>& networkReader = std::make_shared<NetworkReader>());
	virtual ~TemperatureForecast();

	void updateCache();

	Values getForecastValues(std::time_t from, std::time_t to) const;

	void startTimer(const std::chrono::duration<int64_t>& period);
	void stopTimer();
	virtual void onTimer() override;

	// only for testing
	const std::list<ValuesWithTimes> getContainer() const;

	static std::list<ValuesWithTimes> parseXml(const std::string& text);
	static std::time_t parseTimeString(const std::string& text);
};


class TemperatureForecast::NetworkReader {
public:
	virtual ~NetworkReader() = default;
	virtual std::string read(const std::string& url, const std::string& location, const std::string& appid) const;
};


struct TemperatureForecast::Values {
	const float min;
	const float max;

	Values(float min, float max);

	// for testing
	bool operator== (const Values& other) const;
	friend std::ostream& operator<<(std::ostream& os, const Values& values);
};


struct TemperatureForecast::ValuesWithTimes {
	const std::time_t from;
	const std::time_t to;
	const Values minMaxValues;

	ValuesWithTimes(std::time_t from, std::time_t to, const Values& values);

	// for testing
	bool operator== (const ValuesWithTimes& other) const;
	friend std::ostream& operator<<(std::ostream& os, const ValuesWithTimes& valuesWithTimes);
};
