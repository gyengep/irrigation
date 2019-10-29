#pragma once
#include <gmock/gmock.h>
#include "DocumentView/View.h"


class MockView : public View {
public:
	MockView(Document& document) : View(document) {
	}

	MOCK_METHOD0(initialize, void());
	MOCK_METHOD0(terminate, void());
};
