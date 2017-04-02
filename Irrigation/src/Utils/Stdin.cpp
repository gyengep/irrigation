#include "common.h"
#include "Stdin.h"

#include <sys/select.h>
#include <stdio.h>


std::unique_ptr<Stdin> Stdin::instance;

Stdin& Stdin::getInstance() {
	if (!instance) {
		instance.reset(new Stdin());
	}

	return *instance;
}

Stdin::Stdin() {
	buffer = NULL;
	bufferSize = 0;

	tv.tv_sec = 0;
	tv.tv_usec = 100000; // 0.1 sec
}

Stdin::~Stdin() {
	free(buffer);
}

const char* Stdin::readLine() {
	FD_ZERO(&rfds);
	FD_SET(fileno(stdin), &rfds);

	int retval = select(fileno(stdin) + 1, &rfds, NULL, NULL, &tv);

	if (retval > 0) {
		getline(&buffer, &bufferSize, stdin);
		return buffer;
	}

	return NULL;
}

