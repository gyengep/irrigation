#pragma once
#include <string>
#include <stdexcept>



class XMLParseException : public std::runtime_error {
public:
	XMLParseException(const char* what) :
		std::runtime_error(what) {}
};


class RequiredTagMissing : public XMLParseException {
public:
	RequiredTagMissing(const char* what) :
		XMLParseException(what) {}
};


class RequiredAttributeMissing : public XMLParseException {
public:
	RequiredAttributeMissing(const char* what) :
		XMLParseException(what) {}
};
