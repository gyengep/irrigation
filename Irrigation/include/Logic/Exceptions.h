#pragma once
#include <stdexcept>
#include "IdType.h"



class InvalidRunTimeIdException : public std::runtime_error {
public:
	explicit InvalidRunTimeIdException(IdType id) : std::runtime_error("Invalid runtimeID" + std::to_string(id)) {}
};


class InvalidStartTimeIdException : public std::runtime_error {
public:
	explicit InvalidStartTimeIdException(IdType id) : std::runtime_error("Invalid starttimeID" + std::to_string(id)) {}
};


class InvalidSchedulerException : public std::runtime_error {
public:
	explicit InvalidSchedulerException() : std::runtime_error("Invalid scheduler type") {}
};
