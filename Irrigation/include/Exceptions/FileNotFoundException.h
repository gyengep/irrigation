#pragma once
#include <cstring>
#include <stdexcept>
#include "IOException.h"


class FileNotFoundException : public IOException {
public:
	FileNotFoundException() :
		IOException(ENOENT) {}
};
