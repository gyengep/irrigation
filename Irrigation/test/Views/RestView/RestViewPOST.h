#pragma once
#include <gmock/gmock.h>
#include <list>
#include <utility>
#include "RestViewTest.h"


class RestView_POST : public RestViewTest {
protected:

	struct UrlAllowed {
		const bool postAllowed;
		const bool patchAllowed;
		const bool deleteAllowed;
		const std::string url;

		UrlAllowed(
			bool postAllowed,
			bool patchAllowed,
			bool deleteAllowed,
			const std::string& url
		);
	};

	std::list<UrlAllowed> urls;

    virtual void SetUp();
    virtual void TearDown();
};
