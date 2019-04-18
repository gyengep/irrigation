#pragma once
#include <string>
#include "Exceptions/ParserException.h"



class XMLParseException : public ParserException {
public:
	XMLParseException(const char* what) :
		ParserException(what) {}
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
