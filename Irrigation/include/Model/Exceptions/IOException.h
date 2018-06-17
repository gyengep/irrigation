#pragma once
#include <stdexcept>


class IOException : public std::runtime_error {
public:
	IOException(const char* what) : std::runtime_error(what) {}
};

class FileNotFoundException : public IOException {
public:
	FileNotFoundException(const char* what) : IOException(what) {}
};
