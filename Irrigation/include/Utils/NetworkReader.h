#pragma once
#include <string>


class NetworkReader {

	static size_t writeCallback(char* buffer, size_t size, size_t nmemb, void* ctxt);

public:
	NetworkReader();
	virtual ~NetworkReader();

	virtual std::string read(const std::string& url) const;
};
