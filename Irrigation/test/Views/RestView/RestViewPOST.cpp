#include "RestViewPOST.h"


///////////////////////////////////////////////////////////////////////////////

RestView_POST::UrlAllowed::UrlAllowed(bool postAllowed, bool patchAllowed, bool deleteAllowed, const std::string& url) :
	postAllowed(postAllowed),
	patchAllowed(patchAllowed),
	deleteAllowed(deleteAllowed),
	url(url)
{
}

///////////////////////////////////////////////////////////////////////////////

void RestView_POST::SetUp() {
	RestViewTest::SetUp();

	const IdType programId(25);
	const IdType startTimeId(30);
	const IdType runTimeOd(5);

	urls.push_back(UrlAllowed(false, true, false,  createRunTimeContainerUrl(programId)));
	urls.push_back(UrlAllowed(false, true, true,   createStartTimeUrl(programId, startTimeId)));
	urls.push_back(UrlAllowed(true,  false, false, createStartTimeContainerUrl(programId)));
	urls.push_back(UrlAllowed(false, true, true,   createProgramUrl(programId)));
	urls.push_back(UrlAllowed(true,  false, false, createProgramContainerUrl()));
	urls.push_back(UrlAllowed(false, true, false,  createHotWeatherSchedulerUrl(programId)));
	urls.push_back(UrlAllowed(false, true, false,  createTemperatureDependentSchedulerUrl(programId)));
	urls.push_back(UrlAllowed(false, true, false,  createWeeklySchedulerUrl(programId)));
	urls.push_back(UrlAllowed(false, false, false, createTemperatureTodayUrl()));
	urls.push_back(UrlAllowed(false, false, false, createTemperatureTomorrowUrl()));
	urls.push_back(UrlAllowed(false, false, false, createTemperatureYesterdayUrl()));
	urls.push_back(UrlAllowed(false, false, false, createCurrentTemperatureUrl()));
}

void RestView_POST::TearDown() {
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestView_POST, POST_405_Method_Not_Allowed) {
	for (const auto& item : urls) {
		if (false == item.postAllowed) {
			std::cout << item.url << std::endl;
			checkResponse_405_Method_Not_Allowed(
					POST_ContentType_Xml(item.url, "<aa></aa>")
				);
		}
	}
}

TEST_F(RestView_POST, POST_400_Bad_Request) {
	for (const auto& item : urls) {
		if (true == item.postAllowed) {
			std::cout << item.url << std::endl;
			checkResponse_400_Bad_Request(
					POST_ContentType_Xml(item.url, "Invalid XML")
				);
		}
	}
}

TEST_F(RestView_POST, POST_415_Unsupported_Media_Type) {
	for (const auto& item : urls) {
		if (true == item.postAllowed) {
			std::cout << item.url << std::endl;
			checkResponse_415_Unsupported_Media_Type(
					POST_ContentType_Json(item.url, "{ \"key\" : \"value\" }")
				);
		}
	}
}

TEST_F(RestView_POST, PATCH_405_Method_Not_Allowed) {
	for (const auto& item : urls) {
		if (false == item.patchAllowed) {
			std::cout << item.url << std::endl;
			checkResponse_405_Method_Not_Allowed(
					PATCH_ContentType_Xml(item.url, "<aa></aa>")
				);
		}
	}
}

TEST_F(RestView_POST, DELETE_405_Method_Not_Allowed) {
	for (const auto& item : urls) {
		if (false == item.deleteAllowed) {
			std::cout << item.url << std::endl;
			checkResponse_405_Method_Not_Allowed(
					DELETE(item.url)
				);
		}
	}
}

TEST_F(RestView_POST, GET_406_Not_Acceptable) {
	for (const auto& item : urls) {
		std::cout << item.url << std::endl;
		checkResponse_406_Not_Acceptable(
				GET_Accept_Json(item.url)
			);
	}
}
