#pragma once
#include <functional>
#include "Runnable.h"


class ReferenceRunnable : public Runnable {
	Runnable& runnable;

public:
	ReferenceRunnable(Runnable& runnable);
	virtual ~ReferenceRunnable();

	virtual void run() override;
	virtual void interrupt() override;
};
