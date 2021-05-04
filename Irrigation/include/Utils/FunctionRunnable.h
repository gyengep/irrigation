#pragma once
#include <functional>
#include "Runnable.h"


class FunctionRunnable : public Runnable {
	const std::function<void()> func;

public:
	FunctionRunnable(const std::function<void()>& func);
	virtual ~FunctionRunnable();

	virtual void run() override;
};
