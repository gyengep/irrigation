#pragma once
#include <string>
#include "Utils/ShutdownManager.h"


class ShutdownManagerImpl : public ShutdownManager {
	enum class Task {
		None,
		Poweroff,
		Reboot
	};

	Task task;

	void execute(const std::string& command);

public:
	ShutdownManagerImpl();
	virtual ~ShutdownManagerImpl();

	virtual void powerOff() override;
	virtual void reboot() override;
};
