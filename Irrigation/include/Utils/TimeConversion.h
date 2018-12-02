#pragma once
#include <ctime>


std::tm toCalendarTime(int year, int month, int day, int hour, int min, int sec);
std::tm toCalendarTime(int year, int month, int day);

unsigned getElapsedDaysSinceEpoch(const std::tm& timeinfo);
