#pragma once
#include <chrono>
#include <utility>


std::pair<std::time_t, std::time_t> getPreviousPeriod(const std::time_t& rawTime, const std::chrono::seconds& periodLength);
std::pair<std::time_t, std::time_t> getCurrentPeriod(const std::time_t& rawTime, const std::chrono::seconds& periodLength);
