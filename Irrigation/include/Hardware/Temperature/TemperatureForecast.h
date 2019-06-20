#pragma once
#include <chrono>
#include <list>
#include <memory>
#include <mutex>
#include <string>
#include "Utils/Timer.h"
#include "TemperatureValues.h"

#ifndef FRIEND_TEST
#define FRIEND_TEST(a, b)
#endif


class TemperatureForecast : public TimerCallback {
	FRIEND_TEST(TemperatureForecastTest, addTemperature);
	FRIEND_TEST(TemperatureForecastTest, addTemperatureInvalid);
	FRIEND_TEST(TemperatureForecastTest, addTemperatureMismatch);
	FRIEND_TEST(TemperatureForecastTest, getForecast);
	FRIEND_TEST(TemperatureForecastTest, getForecastOutOfBounds);

public:
	class  NetworkReader;
	struct MinMaxValues;
	struct MinMaxValuesWithTimes;

private:
	static const std::string url;
	static const std::string location;
	static const std::string appid;

	mutable std::mutex mtx;
	std::list<MinMaxValuesWithTimes> temperatures;
	std::shared_ptr<NetworkReader> networkReader;
	Timer timer;

	void addTemperature(std::time_t from, std::time_t to, const MinMaxValues& values);

	static size_t writeCallback(char* buffer, size_t size, size_t nmemb, void* ctxt);
	static void addTemperatureTo(std::list<MinMaxValuesWithTimes>& temperatures, std::time_t from, std::time_t to, const MinMaxValues& values);

public:
	TemperatureForecast(const std::chrono::duration<int64_t>& updatePeriod);
	TemperatureForecast(const std::chrono::duration<int64_t>& updatePeriod, const std::shared_ptr<NetworkReader>& networkReader);
	virtual ~TemperatureForecast();

	void updateCache();

	MinMaxValues getForecast(std::time_t from, std::time_t to) const;

	void startTimer();
	void stopTimer();
	virtual void onTimer() override;

	// only for testing
	const std::list<MinMaxValuesWithTimes> getContainer() const;

	static std::list<MinMaxValuesWithTimes> parseXml(const std::string& text);
	static std::time_t parseTimeString(const std::string& text);
};


class TemperatureForecast::NetworkReader {
public:
	virtual ~NetworkReader() = default;
	virtual std::string read(const std::string& url, const std::string& location, const std::string& appid) const;
};


struct TemperatureForecast::MinMaxValues {
	const float min;
	const float max;

	MinMaxValues(float min, float max);

	// for testing
	bool operator== (const MinMaxValues& other) const;
	friend std::ostream& operator<<(std::ostream& os, const MinMaxValues& values);
};


struct TemperatureForecast::MinMaxValuesWithTimes {
	const std::time_t from;
	const std::time_t to;
	const MinMaxValues minMaxValues;

	MinMaxValuesWithTimes(std::time_t from, std::time_t to, const MinMaxValues& values);

	// for testing
	bool operator== (const MinMaxValuesWithTimes& other) const;
	friend std::ostream& operator<<(std::ostream& os, const MinMaxValuesWithTimes& valuesWithTimes);
};
