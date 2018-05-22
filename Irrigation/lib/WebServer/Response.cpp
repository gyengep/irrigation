#include "Common.h"
#include "Response.h"
#include "Request.h"

#include <cstring>


Response::Response(const Request& request, const std::string& message, int statusCode/* = MHD_HTTP_OK*/) :
	response(NULL),
	connection(request.getConnection()),
	statusCode(statusCode)
{
	response = MHD_create_response_from_buffer(message.length(), const_cast<char*>(message.c_str()), MHD_RESPMEM_MUST_COPY);
}

Response::~Response() {
	if (response) {
		MHD_destroy_response(response);
	}
}

void Response::send() {
	if (NULL == response) {
		throw std::runtime_error("Can not create response");
	}

	int ret = MHD_queue_response(connection, statusCode, response);

	if (MHD_YES != ret) {
		throw std::runtime_error("Can not send response");
	}
}
