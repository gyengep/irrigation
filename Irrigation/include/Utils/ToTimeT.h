#pragma once
#include <ctime>



//std::time_t toTime(int year, int month, int day, int hour, int min, int sec, bool dst);
std::tm toCalendarTime(int year, int month, int day, int hour, int min, int sec);
std::tm toCalendarTime(int year, int month, int day);
