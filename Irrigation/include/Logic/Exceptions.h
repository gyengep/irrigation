#pragma once
#include <stdexcept>
#include "IdType.h"



class ProgramIdExist : public std::runtime_error {
public:
	explicit ProgramIdExist(IdType id) : std::runtime_error("ProgramID already exists" + std::to_string(id)) {}
};


class InvalidProgramIdException : public std::runtime_error {
public:
	explicit InvalidProgramIdException(IdType id) : std::runtime_error("Invalid programID" + std::to_string(id)) {}
};


class InvalidRunTimeIdException : public std::runtime_error {
public:
	explicit InvalidRunTimeIdException(IdType id) : std::runtime_error("Invalid runTimeID" + std::to_string(id)) {}
};


class InvalidStartTimeIdException : public std::runtime_error {
public:
	explicit InvalidStartTimeIdException(IdType id) : std::runtime_error("Invalid startTimeID" + std::to_string(id)) {}
};


class StartTimeIdExist : public std::runtime_error {
public:
	explicit StartTimeIdExist(IdType id) : std::runtime_error("StartTimeID already exists" + std::to_string(id)) {}
};


class InvalidSchedulerException : public std::runtime_error {
public:
	explicit InvalidSchedulerException() : std::runtime_error("Invalid scheduler type") {}
};
