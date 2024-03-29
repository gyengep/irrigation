#include "RestServerTest.h"
#include "TestCommon/CreateUrl.h"
#include "Utils/CurlStringReader.h"
#include "WebServer/KeyValue.h"
#include "Mocks/MockFileWriter.h"
#include <curl/curl.h>
#include <functional>

using namespace testing;
using namespace std::placeholders;


void RestServerTest::SetUp() {
	restService.reset(new RestService());

	restService->addPath("PUT", "/programs", bind(&RestServerTest::onCreateProgram, this, _1, _2));
	restService->addPath("GET", "/programs/{programID}", bind(&RestServerTest::onGetProgram, this, _1, _2));

	webServer.reset(new WebServer(restService, port, std::make_shared<NiceMock<MockFileWriterFactory>>()));
	webServer->start();
}

void RestServerTest::TearDown() {
	webServer->stop();
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestServerTest, invalidPath) {
	EXPECT_ANY_THROW(restService->addPath("GET", "//programs", std::bind(&RestServerTest::onCreateProgram, this, _1, _2)));
}

TEST_F(RestServerTest, callbackWithoutParameter) {
	const std::string url = createUrl(port, "/programs");

	CURL *curl = curl_easy_init();

	ASSERT_THAT(curl, NotNull());
	EXPECT_CALL(*this, onCreateProgram(_, KeyValue())).Times(1);

	CurlStringReader curlStringReader("");

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_PUT, 1);
	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, CurlStringReader::readFunction);
	curl_easy_setopt(curl, CURLOPT_READDATA, &curlStringReader);
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
}

TEST_F(RestServerTest, callbackWithParameter) {
	const std::string url = createUrl(port, "/programs/567");

	CURL *curl = curl_easy_init();

	ASSERT_THAT(curl, NotNull());
	EXPECT_CALL(*this, onGetProgram(_, KeyValue({{"programID", "567"}}))).Times(1);

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
}
