#include "WebServerTest.h"
#include "CurlCallbacks/CurlCallbacks.h"
#include "Logger/Logger.h"
#include <curl/curl.h>

using namespace std;
using namespace testing;
using testing::AllOf;


void WebServerTest::SetUp() {
	logOss.reset(new ostringstream());

	//LOGGER.setLevel(LogLevel::TRACE);
	LOGGER.setOutputStream(logOss);
	testWebService.reset(new TestWebService());
	webServer.reset(new WebServer(testWebService, port));

	webServer->start();
}

void WebServerTest::TearDown() {
	webServer->stop();
	//cout << "LOG: " << logOss->str() << endl;
}

WebServerTest::TestWebService::TestWebService() :
	httpResponseCode(200)
{
}

unique_ptr<HttpResponse> WebServerTest::TestWebService::onRequest(const HttpRequest& request) {
	lastRequestedUrl = request.getUrl();
	lastRequestedMethod = request.getMethod();
	lastRequestedVersion = request.getVersion();
	lastRequestedParameters = request.getParameters();
	lastRequestedHeaders = request.getHeaders();
	lastRequestedData = *request.getUploadData();

	return unique_ptr<HttpResponse>(new HttpResponse(httpResponse, httpResponseHeaders, httpResponseCode));
}


TEST_F(WebServerTest, connectGET) {
	const string expectedPath = "/connectGET";
	const string url = createUrl(port, expectedPath, KeyValue());

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	EXPECT_THAT(testWebService->lastRequestedUrl, Eq(expectedPath));
	EXPECT_THAT(testWebService->lastRequestedMethod, Eq("GET"));
	EXPECT_THAT(testWebService->lastRequestedVersion, Eq("HTTP/1.1"));
}

TEST_F(WebServerTest, connectPOST) {
	const string expectedPath = "/connectPOST";
	const string url = createUrl(port, expectedPath, KeyValue());

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
        curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	EXPECT_THAT(testWebService->lastRequestedUrl, Eq(expectedPath));
	EXPECT_THAT(testWebService->lastRequestedMethod, Eq("POST"));
	EXPECT_THAT(testWebService->lastRequestedVersion, Eq("HTTP/1.1"));
}

TEST_F(WebServerTest, connectPATCH) {
	const string expectedPath = "/connectPATCH";
	const string url = createUrl(port, expectedPath, KeyValue());

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 0L);
        curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	EXPECT_THAT(testWebService->lastRequestedUrl, Eq(expectedPath));
	EXPECT_THAT(testWebService->lastRequestedMethod, Eq("PATCH"));
	EXPECT_THAT(testWebService->lastRequestedVersion, Eq("HTTP/1.1"));
}

TEST_F(WebServerTest, connectDELETE) {
	const string expectedPath = "/connectDELETE";
	const string url = createUrl(port, expectedPath, KeyValue());

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	EXPECT_THAT(testWebService->lastRequestedUrl, Eq(expectedPath));
	EXPECT_THAT(testWebService->lastRequestedMethod, Eq("DELETE"));
	EXPECT_THAT(testWebService->lastRequestedVersion, Eq("HTTP/1.1"));
}

TEST_F(WebServerTest, resultOK) {
	const string path = "/result200";
	const string url = createUrl(port, path, KeyValue());

	const int expectedHttpResponseCode = 200;
	const char* expectedHttpResponse = "TEST_RESPONE";

	testWebService->httpResponse = expectedHttpResponse;
	testWebService->httpResponseCode = expectedHttpResponseCode;

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());

    long actualHttpCode = 0;
    WriteCallbackData writeCallbackData;

    if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &writeCallbackData);
        curl_easy_perform(curl);
	    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &actualHttpCode);
		curl_easy_cleanup(curl);
	}

	EXPECT_THAT(actualHttpCode, Eq(expectedHttpResponseCode));
	EXPECT_THAT(writeCallbackData.text, Eq(expectedHttpResponse));
}

TEST_F(WebServerTest, result404) {
	const string path = "/result404";
	const string url = createUrl(port, path, KeyValue());

	const int expectedHttpResponseCode = 404;
	const char* expectedHttpResponse = "NOT FOUND";

	testWebService->httpResponse = expectedHttpResponse;
	testWebService->httpResponseCode = expectedHttpResponseCode;

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());

    long actualHttpCode = 0;
    WriteCallbackData writeCallbackData;

    if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &writeCallbackData);
        curl_easy_perform(curl);
	    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &actualHttpCode);
		curl_easy_cleanup(curl);
	}

	EXPECT_THAT(actualHttpCode, Eq(expectedHttpResponseCode));
	EXPECT_THAT(writeCallbackData.text, Eq(expectedHttpResponse));
}

TEST_F(WebServerTest, requestParameters) {
	const string path = "/requestParameters";
	const KeyValue expectedParameters {{"name", "Tom"}, {"value", "523"}, {"project", "curl"}};
	const string url = createUrl(port, path, expectedParameters);

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	EXPECT_THAT(testWebService->lastRequestedParameters, Eq(expectedParameters));
}

TEST_F(WebServerTest, noRequestParameters) {
	const string path = "/noRequestParameters";
	const KeyValue expectedParameters;
	const string url = createUrl(port, path, expectedParameters);

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	EXPECT_THAT(testWebService->lastRequestedParameters, Eq(expectedParameters));
}

TEST_F(WebServerTest, uploadDataPOST) {
	const string path = "/uploadDataPost";
	const string url = createUrl(port, path, KeyValue());
    const string uploadData = "POST_UPLOAD_DATA";

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());

    if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, uploadData.c_str());
        curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	EXPECT_THAT(testWebService->lastRequestedData, ContainerEq(ByteBuffer(uploadData.begin(), uploadData.end())));
}

TEST_F(WebServerTest, uploadDataPATCH) {
	const string expectedPath = "/uploadDataPATCH";
	const string url = createUrl(port, expectedPath, KeyValue());
    const string uploadData = "PATCH_UPLOAD_DATA";

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());

	ReadCallbackData readCallbackData(uploadData);

    if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, readCallback);
		curl_easy_setopt(curl, CURLOPT_READDATA, &readCallbackData);
        curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

    EXPECT_THAT(testWebService->lastRequestedData, Eq(ByteBuffer(uploadData.begin(), uploadData.end())));
}

TEST_F(WebServerTest, requestHeader) {
	const string expectedPath = "/requestHeader";
	const string url = createUrl(port, expectedPath, KeyValue());

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());

	if (curl) {
		struct curl_slist *list = NULL;

		list = curl_slist_append(list, "Shoesize: 10");
		list = curl_slist_append(list, "HeaderKey: HeaderValue");

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

        curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		curl_slist_free_all(list);
	}

	EXPECT_THAT(
		testWebService->lastRequestedHeaders,
		AllOf(
			Contains(Pair("Shoesize", "10")),
			Contains(Pair("HeaderKey", "HeaderValue"))
		)
	);
}

TEST_F(WebServerTest, responseHeader) {
	const string expectedPath = "/responseHeader";
	const string url = createUrl(port, expectedPath, KeyValue());

	const pair<string, string> h1 {"h1", "v1"};
	const pair<string, string> h2 {"headerName", "headerValue"};
	const pair<string, string> h3 {"1234", "abc"};

	testWebService->httpResponseHeaders.insert(h1);
	testWebService->httpResponseHeaders.insert(h2);
	testWebService->httpResponseHeaders.insert(h3);

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());

    HeaderCallbackData headerCallbackData;

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, headerCallback);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &headerCallbackData);

        curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	EXPECT_THAT(
		headerCallbackData.headers,
		AllOf(
			Contains(h1.first + ": " + h1.second + "\r\n"),
			Contains(h2.first + ": " + h2.second + "\r\n"),
			Contains(h3.first + ": " + h3.second + "\r\n")
		)
	);
}

