#include "Valve.h"


class FakeValve: public Valve {
public:
	FakeValve() = default;
	virtual ~FakeValve() = default;

	virtual void activate() {}
	virtual void deactivate() {}
};

///////////////////////////////////////////////////////////////////////////////

class FakeValveFactory : public ValveFactory {
public:
	virtual std::unique_ptr<Valve> createValve(size_t) {
		return std::unique_ptr<Valve>(new FakeValve());
	}
};
