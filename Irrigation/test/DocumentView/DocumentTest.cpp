#include <gmock/gmock.h>
#include <stdexcept>
#include "DocumentView/Document.h"
#include "DocumentView/View.h"

using namespace std;



class MockView : public View {
public:

	MockView(Document& document) : View(document) {
	}

	MOCK_METHOD0(initialize, void());
	MOCK_METHOD0(terminate, void());
};


TEST(DocumentTest, addView) {
	Document document;
	MockView* view = new MockView(document);

	EXPECT_CALL(*view, initialize()).Times(1);
	EXPECT_CALL(*view, terminate()).Times(1);

	document.addView(view);
}

TEST(DocumentTest, addViewWithInvalidDocument) {
	Document document1;
	Document document2;
	MockView* view = new MockView(document1);

	EXPECT_THROW(document2.addView(view), logic_error);
}

TEST(DocumentTest, getDocument) {
	Document document;
	View view(document);

	EXPECT_EQ(&document, &view.getDocument());
}
