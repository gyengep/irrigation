#pragma once
#include <string>


class NetworkReader {
public:
	NetworkReader();
	virtual ~NetworkReader();

	virtual std::string read(const std::string& url) const;
};
