#include "ReferenceRunnable.h"


ReferenceRunnable::ReferenceRunnable(Runnable& runnable) :
	runnable(runnable)
{
}

ReferenceRunnable::~ReferenceRunnable() {
}

void ReferenceRunnable::run() {
	runnable.run();
}

void ReferenceRunnable::interrupt() {
	runnable.interrupt();
}

