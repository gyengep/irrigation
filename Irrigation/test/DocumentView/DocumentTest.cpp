#include <gmock/gmock.h>
#include <stdexcept>
#include "DocumentView/Document.h"
#include "MockView.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////

TEST(DocumentTest, addView) {
	Document document;
	unique_ptr<MockView> view(new MockView(document));

	EXPECT_CALL(*view, initialize()).Times(1);
	EXPECT_CALL(*view, terminate()).Times(1);

	document.addView(move(view));
}

TEST(DocumentTest, addViewWithInvalidDocument) {
	Document document1;
	Document document2;
	unique_ptr<MockView> view(new MockView(document1));

	EXPECT_THROW(document2.addView(move(view)), logic_error);
}

TEST(DocumentTest, getDocument) {
	Document document;
	View view(document);

	EXPECT_EQ(&document, &view.getDocument());
}
