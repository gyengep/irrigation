#include "HttpRequest.h"
#include "WebServer.h"
#include <stdexcept>
#include "Logger/Logger.h"


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

void HttpRequest::initParameters() const {
	if (nullptr == parameters.get()) {
		const_cast<std::unique_ptr<KeyValue>&>(parameters).reset(new KeyValue());
		const_cast<std::unique_ptr<std::string>&>(parametersAsText).reset(new std::string());

		if (nullptr != connection) {
			ParametersHelperType aaa(parameters.get(), parametersAsText.get());
			MHD_get_connection_values(connection, MHD_GET_ARGUMENT_KIND, iterateOnValuesParameters, &aaa);
		}
	}
}

const std::string& HttpRequest::getParametersAsText() const {
	initParameters();
	return *parametersAsText;
}

const KeyValue& HttpRequest::getParameters() const {
	initParameters();
	return *parameters;
}

int HttpRequest::iterateOnValuesParameters(void *cls, enum MHD_ValueKind kind, const char *key, const char *value) {
/*
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

		LOGGER.trace("iterate on HttpRequest%s: \"%s\" - \"%s\"", kindText, key, value);
	}
*/

	ParametersHelperType* aaa = static_cast<ParametersHelperType*>(cls);

	KeyValue* parameters = aaa->first;
	std::string* parametersAsText = aaa->second;

	const std::string a1 = std::string(key);
	const std::string a2 = std::string(value);

	parameters->insert(std::make_pair(a1, a2));

	if (false == parametersAsText->empty()) {
		parametersAsText->append("&");
	}
	parametersAsText->append(a1 + "=" + a2);

	return MHD_YES;
}

const KeyValue& HttpRequest::getHeaders() const {
	if (nullptr == headers.get()) {
		const_cast<std::unique_ptr<KeyValue>&>(headers).reset(new KeyValue());

		if (nullptr != connection) {
			MHD_get_connection_values(connection, MHD_HEADER_KIND, iterateOnValuesHeaders, headers.get());
		}
	}

	return *headers;
}

int HttpRequest::iterateOnValuesHeaders(void *cls, enum MHD_ValueKind kind, const char *key, const char *value) {
/*
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

		LOGGER.trace("iterate on HttpRequest%s: \"%s\" - \"%s\"", kindText, key, value);
	}
*/
	KeyValue* parameters = static_cast<KeyValue*>(cls);
	parameters->insert(std::make_pair(std::string(key), std::string(value)));
	return MHD_YES;
}

