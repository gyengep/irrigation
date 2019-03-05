#include "RestViewTest.h"
#include "Dto2Object/ProgramSamples.h"
#include "CurlCallbacks/CurlCallbacks.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "Logic/Program.h"
#include "Logic/ProgramContainer.h"
#include "Logic/RunTime.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTime.h"
#include "Logic/StartTimeContainer.h"
#include "Model/IrrigationDocument.h"
#include "Views/RestView/RestView.h"
#include <curl/curl.h>

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;

const string urlPrefix = "";
const uint16_t port = 8080;


void RestViewTest::SetUp() {
	document = IrrigationDocument::Builder().build();
	document->addView(unique_ptr<View>(new RestView(*document, port)));
}

void RestViewTest::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, getRunTimes) {

	const shared_ptr<Program> program = Program::Builder().build();
	const IdType id = IdType();
	const string path = "/programs/" + to_string(id) + "/runtimes";
	const string url = createUrl(port, path, KeyValue());

	document->getPrograms().insert(id, ProgramSample1().getObject());

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());

    long actualResponseCode = 0;
    WriteCallbackData writeCallbackData;

    if (curl) {
    	cout << "CURL OK" << endl;
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &writeCallbackData);
        curl_easy_perform(curl);
	    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &actualResponseCode);
		curl_easy_cleanup(curl);
	} else {
    	cout << "CURL NOK" << endl;
	}

	EXPECT_THAT(actualResponseCode, Eq(200));
	EXPECT_THAT(writeCallbackData.text, Eq(XmlWriter().save(ProgramSample1().getDto().getRunTimes())));
}

TEST_F(RestViewTest, getStartTimes) {

	const shared_ptr<Program> program = Program::Builder().build();
	const IdType id = IdType();
	const string path = "/programs/" + to_string(id) + "/starttimes";
	const string url = createUrl(port, path, KeyValue());

	document->getPrograms().insert(id, ProgramSample1().getObject());

	CURL *curl = curl_easy_init();
	ASSERT_THAT(curl, NotNull());

    long actualResponseCode = 0;
    WriteCallbackData writeCallbackData;

    if (curl) {
    	cout << "CURL OK" << endl;
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &writeCallbackData);
        curl_easy_perform(curl);
	    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &actualResponseCode);
		curl_easy_cleanup(curl);
	} else {
    	cout << "CURL NOK" << endl;
	}

	EXPECT_THAT(actualResponseCode, Eq(200));
	EXPECT_THAT(writeCallbackData.text, Eq(XmlWriter().save(ProgramSample1().getDto().getStartTimes())));
}
