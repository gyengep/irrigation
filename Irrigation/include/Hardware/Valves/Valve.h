#pragma once
#include <memory>


class Valve {
public:
	virtual ~Valve() = default;
	virtual void activate() = 0;
	virtual void deactivate() = 0;
};

///////////////////////////////////////////////////////////////////////////////

class ValveFactory {
public:
	virtual ~ValveFactory() = default;
	virtual std::shared_ptr<Valve> createValve(size_t id) = 0;
};
