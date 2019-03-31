#pragma once
#include <string>
#include <memory>


class ErrorWriter {
public:
	virtual ~ErrorWriter() = default;
	virtual std::string contentType() const = 0;
	virtual std::string toString(unsigned errorCode, const std::string& errorMessage) const = 0;
};
