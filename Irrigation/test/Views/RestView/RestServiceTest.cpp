#include "RestServiceTest.h"
#include "Views/RestView/RestServiceException.h"
#include "WebServer/HttpResponse.h"

using namespace std;
using namespace testing;
using namespace placeholders;


void RestServiceTest::SetUp() {
	restService.addPath(MHD_HTTP_METHOD_GET, "/tom/and/jerry", bind(&RestServiceTest::callback1, this, _1, _2));
	restService.addPath(MHD_HTTP_METHOD_DELETE, "/tom/and/jerry", bind(&RestServiceTest::callback2, this, _1, _2));
	restService.addPath(MHD_HTTP_METHOD_POST, "/resource1/{value1}/resource2/{value2}", bind(&RestServiceTest::callback3, this, _1, _2));
}

void RestServiceTest::TearDown() {
}


TEST_F(RestServiceTest, notFound) {
	try {
		HttpRequest request(nullptr, MHD_HTTP_VERSION_1_1, MHD_HTTP_METHOD_GET, "/123456789", shared_ptr<ByteBuffer>(new ByteBuffer()));
		restService.onRequest(request);
		FAIL();
	} catch (RestServiceException& e) {
		EXPECT_EQ(MHD_HTTP_NOT_FOUND, e.getStatusCode());
	} catch (...) {
		FAIL();
	}
}

TEST_F(RestServiceTest, methodNotAllowed) {
	try {
		HttpRequest request(nullptr, MHD_HTTP_VERSION_1_1, MHD_HTTP_METHOD_POST, "/tom/and/jerry", shared_ptr<ByteBuffer>(new ByteBuffer()));
		restService.onRequest(request);
		FAIL();
	} catch (RestServiceException& e) {
		EXPECT_EQ(MHD_HTTP_METHOD_NOT_ALLOWED, e.getStatusCode());
	} catch (...) {
		FAIL();
	}
}

TEST_F(RestServiceTest, testCallback) {
	EXPECT_CALL(*this, callback1(_, KeyValue())).Times(1);

	HttpRequest request1(nullptr, MHD_HTTP_VERSION_1_1, MHD_HTTP_METHOD_GET, "/tom/and/jerry", shared_ptr<ByteBuffer>(new ByteBuffer()));
	restService.onRequest(request1);
}

TEST_F(RestServiceTest, testParameters) {
	EXPECT_CALL(*this, callback3(_, KeyValue({{"value1", "12345"}, {"value2", "ASDFG"}}))).Times(1);

	HttpRequest request1(nullptr, MHD_HTTP_VERSION_1_1, MHD_HTTP_METHOD_POST, "/resource1/12345/resource2/ASDFG", shared_ptr<ByteBuffer>(new ByteBuffer()));
	restService.onRequest(request1);
}
