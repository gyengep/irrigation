#pragma once
#include <memory>


class Stdin {
	static std::unique_ptr<Stdin> instance;

	fd_set rfds;
	timeval tv;
	char* buffer;
	size_t bufferSize;

	Stdin();

public:

	static Stdin& getInstance();

	~Stdin();
	const char* readLine();
};
