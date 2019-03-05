#pragma once
#include <string>
#include <memory>


class ErrorWriter {
public:
	virtual ~ErrorWriter() = default;
	virtual std::string to(const std::string& errorMessage, unsigned errorCode) = 0;
};


class ErrorWriterFactory {
public:
	virtual ~ErrorWriterFactory() = default;
	virtual std::unique_ptr<ErrorWriter> create() const = 0;
};

