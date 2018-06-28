#pragma once
#include <memory>
#include <mutex>


class GpioHandler {
	static std::mutex createMutex;
	static std::shared_ptr<GpioHandler> instance;

public:
	GpioHandler() {}
	virtual ~GpioHandler() {}

	virtual void setPin(int pin, int mode);

	static void init();
	static const std::shared_ptr<GpioHandler> getInstancePtr();
};
