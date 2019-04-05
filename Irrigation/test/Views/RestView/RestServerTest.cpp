#include "RestServerTest.h"
#include "CurlCallbacks/CurlCallbacks.h"
#include "WebServer/KeyValue.h"
#include <curl/curl.h>
#include <functional>

using namespace std;
using namespace testing;
using namespace placeholders;


void RestServerTest::SetUp() {
	restService.reset(new RestService());

	restService->addPath("PUT", "/programs", bind(&RestServerTest::onCreateProgram, this, _1, _2));
	restService->addPath("GET", "/programs/{programID}", bind(&RestServerTest::onGetProgram, this, _1, _2));

	webServer.reset(new WebServer(restService, port));
	webServer->start();
}

void RestServerTest::TearDown() {
	webServer->stop();
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestServerTest, invalidPath) {
	EXPECT_ANY_THROW(restService->addPath("GET", "/programs/", bind(&RestServerTest::onCreateProgram, this, _1, _2)));
}

TEST_F(RestServerTest, callbackWithoutParameter) {
	const string url = createUrl(port, "/programs");

	CURL *curl = curl_easy_init();

	ASSERT_NE(nullptr, curl);
	EXPECT_CALL(*this, onCreateProgram(_, KeyValue())).Times(1);

	if (curl) {
		ReadCallbackData readCallbackData("");

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_PUT, 1);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, readCallback);
		curl_easy_setopt(curl, CURLOPT_READDATA, &readCallbackData);
        curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
}

TEST_F(RestServerTest, callbackWithParameter) {
	const string url = createUrl(port, "/programs/567");

	CURL *curl = curl_easy_init();

	ASSERT_NE(nullptr, curl);
	EXPECT_CALL(*this, onGetProgram(_, KeyValue({{"programID", "567"}}))).Times(1);

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
}
