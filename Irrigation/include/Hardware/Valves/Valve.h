#pragma once


class Valve {
public:
	virtual ~Valve() = default;
	virtual void activate() = 0;
	virtual void deactivate() = 0;
};
