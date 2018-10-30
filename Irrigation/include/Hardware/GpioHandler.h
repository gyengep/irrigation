#pragma once
#include <memory>


class GpioHandler {
	GpioHandler();

public:
	~GpioHandler();

	void setPin(int pin, int mode);

	static void init();
	static GpioHandler& getInstance();
};
