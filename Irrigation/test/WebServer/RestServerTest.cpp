#include "RestServerTest.h"
#include "WebServerTest.h"
#include <functional>
#include <curl/curl.h>

using namespace std;
using namespace testing;
using namespace placeholders;


void RestServerTest::SetUp() {
	restService.reset(new RestService());

	restService->addPath("POST", "/programs", bind(onCreateProgram, this, _1, _2));
	restService->addPath("GET", "/programs/{programID}", bind(onGetProgram, this, _1, _2));

	webServer.reset(new WebServer(restService, port));
	webServer->start();
}

void RestServerTest::TearDown() {
	webServer->stop();
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestServerTest, invalidPath) {
	EXPECT_ANY_THROW(restService->addPath("GET", "/programs/", bind(onCreateProgram, this, _1, _2)));
}

TEST_F(RestServerTest, callbackWithoutParameter) {
	const string path = "/programs";
	const string url = WebServerTest::createUrl(port, path, Parameters());

	CURL *curl = curl_easy_init();

	ASSERT_NE(nullptr, curl);
	EXPECT_CALL(*this, onCreateProgram(_, _)).Times(1);

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
        curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
}

TEST_F(RestServerTest, callbackWithParameter) {
	const string path = "/programs/567";
	const string url = WebServerTest::createUrl(port, path, Parameters());

	CURL *curl = curl_easy_init();

	ASSERT_NE(nullptr, curl);
	EXPECT_CALL(*this, onGetProgram(_, Parameters({{"programID", "567"}}))).Times(1);

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
}
