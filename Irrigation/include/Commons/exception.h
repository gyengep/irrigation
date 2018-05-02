/*
 * exception.h
 *
 *  Created on: 2016. júl. 1.
 *      Author: Rendszergazda
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <stdexcept>

#define DECLARE_EXCEPTION(CLASS_NAME, BASE_CLASS, MESSAGE)		\
class CLASS_NAME : public BASE_CLASS {							\
public:															\
	explicit CLASS_NAME() : BASE_CLASS(MESSAGE) {}				\
};


class IrrigationException : public std::runtime_error {
public:
	explicit IrrigationException() : std::runtime_error("") {}
	explicit IrrigationException(const std::string& what_arg) : std::runtime_error(what_arg) {}
	explicit IrrigationException(const char* what_arg) : std::runtime_error(what_arg) {}
};


class InvalidIdException : public IrrigationException {
public:
	//explicit InvalidIdException() : IrrigationException() {}
	explicit InvalidIdException(const char* what_arg) : IrrigationException(what_arg) {}
};


class CommandException : public IrrigationException {
public:
	explicit CommandException(const std::string& what_arg) : IrrigationException(what_arg) {}
	explicit CommandException(const char* what_arg) : IrrigationException(what_arg) {}
};


DECLARE_EXCEPTION(InvalidRunTimeIdException, InvalidIdException, "Invalid runtimeID");
DECLARE_EXCEPTION(InvalidStartTimeIdException, InvalidIdException, "Invalid starttimeID");
DECLARE_EXCEPTION(InvalidProgramIdException, InvalidIdException, "Invalid programID");
DECLARE_EXCEPTION(InvalidValveIdException, InvalidIdException, "Invalid valveID");
DECLARE_EXCEPTION(InvalidZoneIdException, InvalidIdException, "Invalid zoneID");
DECLARE_EXCEPTION(InvalidSchedulerException, InvalidIdException, "Invalid scheduler type");
DECLARE_EXCEPTION(InvalidDayException, InvalidIdException, "Invalid day");


DECLARE_EXCEPTION(TooFewArgumentsException, CommandException, "Too few arguments");
DECLARE_EXCEPTION(TooMuchArgumentsException, CommandException, "Too much arguments");
DECLARE_EXCEPTION(InvalidParameterException, CommandException, "Invalid parameter");


class UnknownCommandException : public CommandException {
public:
	UnknownCommandException(const std::string& command) :
		CommandException(std::string("Unknown command: '") + command + "'") {}
};


#endif /* EXCEPTION_H_ */
