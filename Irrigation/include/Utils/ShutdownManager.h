#pragma once


class ShutdownManager {
public:
	virtual ~ShutdownManager() = default;
	virtual void powerOff() = 0;
	virtual void reboot() = 0;
};
