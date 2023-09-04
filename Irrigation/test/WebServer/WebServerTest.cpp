#include "WebServerTest.h"
#include "TestCommon/CreateUrl.h"
#include "Utils/CurlHeaderWriter.h"
#include "Utils/CurlStringReader.h"
#include "Utils/CurlStringWriter.h"
#include <curl/curl.h>

using namespace testing;


void WebServerTest::SetUp() {
	testWebService = std::make_shared<TestWebService>();
	mockFileWriter = std::make_shared<StrictMock<MockFileWriter>>();
	mockFileWriterFactory = std::make_shared<StrictMock<MockFileWriterFactory>>();

	EXPECT_CALL(*mockFileWriterFactory, create(FileWriter::Type::APPEND)).
			Times(1).
			WillOnce(Return(mockFileWriter));

	webServer.reset(new WebServer(testWebService, port, mockFileWriterFactory));

	webServer->start();
}

void WebServerTest::TearDown() {
	webServer->stop();
}

WebServerTest::TestWebService::TestWebService() :
	httpResponseCode(200)
{
}

std::unique_ptr<HttpResponse> WebServerTest::TestWebService::onRequest(const HttpRequest& request) {
	lastRequestedUrl = request.getUrl();
	lastRequestedMethod = request.getMethod();
	lastRequestedVersion = request.getVersion();
	lastRequestedParameters = request.getParameters();
	lastRequestedHeaders = request.getHeaders();
	lastRequestedData = *request.getUploadData();

	return HttpResponse::Builder().
			setStatus(httpResponseCode, "").
			setBody(httpResponseBody).
			addHeaders(httpResponseHeaders).
			build();
}


TEST_F(WebServerTest, connectGET) {
	const std::string expectedPath = "/connectGET";
	const std::string url = createUrl(port, expectedPath);

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());
	EXPECT_CALL(*mockFileWriter, write(AllOf(HasSubstr("GET"), HasSubstr("HTTP/1.1"))));

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
	const std::string expectedPath = "/connectPOST";
	const std::string url = createUrl(port, expectedPath);

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());
	EXPECT_CALL(*mockFileWriter, write(AllOf(HasSubstr("POST"), HasSubstr("HTTP/1.1"))));

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
	const std::string expectedPath = "/connectPATCH";
	const std::string url = createUrl(port, expectedPath);

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());
	EXPECT_CALL(*mockFileWriter, write(AllOf(HasSubstr("PATCH"), HasSubstr("HTTP/1.1"))));

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
	const std::string expectedPath = "/connectDELETE";
	const std::string url = createUrl(port, expectedPath);

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());
	EXPECT_CALL(*mockFileWriter, write(AllOf(HasSubstr("DELETE"), HasSubstr("HTTP/1.1"))));

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
	const std::string path = "/result200";
	const std::string url = createUrl(port, path);

	const int expectedHttpResponseCode = 200;
	const char* expectedHttpResponse = "TEST_RESPONE";

	testWebService->httpResponseBody = expectedHttpResponse;
	testWebService->httpResponseCode = expectedHttpResponseCode;

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());
	EXPECT_CALL(*mockFileWriter, write(AllOf(HasSubstr("200"), HasSubstr("HTTP/1.1"))));

    long actualHttpCode = 0;
    CurlStringWriter curlStringWriter;

    if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlStringWriter::writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curlStringWriter);
        curl_easy_perform(curl);
	    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &actualHttpCode);
		curl_easy_cleanup(curl);
	}

	EXPECT_THAT(actualHttpCode, Eq(expectedHttpResponseCode));
	EXPECT_THAT(curlStringWriter.getText(), Eq(expectedHttpResponse));
}

TEST_F(WebServerTest, result404) {
	const std::string path = "/result404";
	const std::string url = createUrl(port, path);

	const int expectedHttpResponseCode = 404;
	const char* expectedHttpResponse = "NOT FOUND";

	testWebService->httpResponseBody = expectedHttpResponse;
	testWebService->httpResponseCode = expectedHttpResponseCode;

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());
	EXPECT_CALL(*mockFileWriter, write(AllOf(HasSubstr("404"), HasSubstr("HTTP/1.1"))));

    long actualHttpCode = 0;
    CurlStringWriter curlStringWriter;

    if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlStringWriter::writeFunction);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curlStringWriter);
        curl_easy_perform(curl);
	    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &actualHttpCode);
		curl_easy_cleanup(curl);
	}

	EXPECT_THAT(actualHttpCode, Eq(expectedHttpResponseCode));
	EXPECT_THAT(curlStringWriter.getText(), Eq(expectedHttpResponse));
}

TEST_F(WebServerTest, requestParameters) {
	const std::string path = "/requestParameters";
	const KeyValue expectedParameters {{"name", "Tom"}, {"value", "523"}, {"project", "curl"}};
	const std::string url = createUrl(port, path, expectedParameters);

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());
	EXPECT_CALL(
			*mockFileWriter,
			write(
				AllOf(
					HasSubstr("200"),
					HasSubstr("HTTP/1.1"),
					HasSubstr("name=Tom"),
					HasSubstr("value=523"),
					HasSubstr("project=curl")
				)
			)
		);

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	EXPECT_THAT(testWebService->lastRequestedParameters, Eq(expectedParameters));
}

TEST_F(WebServerTest, noRequestParameters) {
	const std::string path = "/noRequestParameters";
	const KeyValue expectedParameters;
	const std::string url = createUrl(port, path, expectedParameters);

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());
	EXPECT_CALL(*mockFileWriter, write(_));

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	EXPECT_THAT(testWebService->lastRequestedParameters, Eq(expectedParameters));
}

TEST_F(WebServerTest, uploadDataPOST) {
	const std::string path = "/uploadDataPost";
	const std::string url = createUrl(port, path);
    const std::string uploadData = "POST_UPLOAD_DATA";

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());
	EXPECT_CALL(*mockFileWriter, write(_));

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
	const std::string expectedPath = "/uploadDataPATCH";
	const std::string url = createUrl(port, expectedPath);
    const std::string uploadData = "PATCH_UPLOAD_DATA";

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());
	EXPECT_CALL(*mockFileWriter, write(_));

	CurlStringReader curlStringReader(uploadData);

    if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, CurlStringReader::readFunction);
		curl_easy_setopt(curl, CURLOPT_READDATA, &curlStringReader);
        curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

    EXPECT_THAT(testWebService->lastRequestedData, Eq(ByteBuffer(uploadData.begin(), uploadData.end())));
}

TEST_F(WebServerTest, requestHeader) {
	const std::string expectedPath = "/requestHeader";
	const std::string url = createUrl(port, expectedPath);

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());
	EXPECT_CALL(*mockFileWriter, write(_));

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
	const std::string expectedPath = "/responseHeader";
	const std::string url = createUrl(port, expectedPath);

	const std::pair<std::string, std::string> h1 {"h1", "v1"};
	const std::pair<std::string, std::string> h2 {"headerName", "headerValue"};
	const std::pair<std::string, std::string> h3 {"1234", "abc"};

	testWebService->httpResponseHeaders.insert(h1);
	testWebService->httpResponseHeaders.insert(h2);
	testWebService->httpResponseHeaders.insert(h3);

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());
	EXPECT_CALL(*mockFileWriter, write(_));

    CurlHeaderWriter curlHeaderWriter;

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, CurlHeaderWriter::headerFunction);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &curlHeaderWriter);

        curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	EXPECT_THAT(
		curlHeaderWriter.getHeaders(),
		AllOf(
			Contains(h1.first + ": " + h1.second + "\r\n"),
			Contains(h2.first + ": " + h2.second + "\r\n"),
			Contains(h3.first + ": " + h3.second + "\r\n")
		)
	);
}

