#pragma once
#include <chrono>
#include <utility>


std::pair<std::chrono::system_clock::time_point, std::chrono::system_clock::time_point>
getPreviousPeriod(const std::chrono::system_clock::time_point& currentTime, const std::chrono::seconds& periodLength);

std::pair<std::chrono::system_clock::time_point, std::chrono::system_clock::time_point>
getCurrentPeriod(const std::chrono::system_clock::time_point& currentTime, const std::chrono::seconds& periodLength);
