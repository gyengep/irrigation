#include "FunctionRunnable.h"


FunctionRunnable::FunctionRunnable(const std::function<void()>& func) :
	func(func)
{
}

FunctionRunnable::~FunctionRunnable() {
}

void FunctionRunnable::run() {
	func();
}
