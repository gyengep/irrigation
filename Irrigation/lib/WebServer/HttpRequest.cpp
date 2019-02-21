#include "HttpRequest.h"
#include "WebServer.h"
#include <stdexcept>
#include "Logger/Logger.h"

using namespace std;


HttpRequest::HttpRequest(MHD_Connection* connection, const char* version, const char* method, const char* url, const std::shared_ptr<ByteBuffer>& uploadData) :
	connection(connection),
	url(url),
	method(method),
	version(version),
	uploadData(uploadData),
	parameters(),
	headers()
{
}

const Parameters& HttpRequest::getParameters() const {
	if (nullptr == parameters.get()) {
		const_cast<std::unique_ptr<Parameters>&>(parameters).reset(new Parameters());

		if (nullptr != connection) {
			MHD_get_connection_values(connection, MHD_GET_ARGUMENT_KIND, iterateOnValues, parameters.get());
		}
	}

	return *parameters;
}

const Parameters& HttpRequest::getHeaders() const {
	if (nullptr == headers.get()) {
		const_cast<std::unique_ptr<Parameters>&>(headers).reset(new Parameters());

		if (nullptr != connection) {
			MHD_get_connection_values(connection, MHD_HEADER_KIND, iterateOnValues, headers.get());
		}
	}

	return *headers;
}

int HttpRequest::iterateOnValues(void *cls, enum MHD_ValueKind kind, const char *key, const char *value) {
	if (LOGGER.isLoggable(LogLevel::TRACE)) {
		const char* kindText;
		switch (kind) {
		case MHD_GET_ARGUMENT_KIND:
			kindText = "Parameters";
			break;
		case MHD_HEADER_KIND:
			kindText = "Headers";
			break;
		default:
			throw logic_error("HttpRequest::iterateOnValues() invalid valueKind: " + to_string(kind));
		}

		LOGGER.trace("iterate on Request%s: \"%s\" - \"%s\"", kindText, key, value);
	}

	Parameters* parameters = static_cast<Parameters*>(cls);
	parameters->insert(std::make_pair(std::string(key), std::string(value)));
	return MHD_YES;
}

