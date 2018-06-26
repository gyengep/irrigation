#include <gmock/gmock.h>
#include <stdexcept>
#include "DocumentView/Document.h"
#include "DocumentView/View.h"

using namespace std;



class MockView : public View {
public:

	MockView() {
		EXPECT_CALL(*this, destructorIsCalled()).Times(1);
	}

	virtual ~MockView() { destructorIsCalled(); }

	MOCK_METHOD0(init, void());
	MOCK_METHOD0(terminate, void());
	MOCK_METHOD0(destructorIsCalled, void());
};


TEST(DocumentTest, addView) {
	Document document;
	MockView* view = new MockView();

	EXPECT_CALL(*view, init()).Times(1);
	EXPECT_CALL(*view, terminate());

	document.addView(view);
}

TEST(DocumentTest, removeView) {
	Document document;
	MockView* view = new MockView();

	EXPECT_CALL(*view, init());
	EXPECT_CALL(*view, terminate()).Times(1);

	document.addView(view);
	document.removeView(view);
}

TEST(DocumentTest, removeViewInvalid) {
	Document document;
	EXPECT_THROW(document.removeView((View*)213132), logic_error);
}

TEST(DocumentTest, getDocument) {
	Document document;
	MockView* view = new MockView();

	EXPECT_CALL(*view, init());
	EXPECT_CALL(*view, terminate());

	document.addView(view);

	EXPECT_EQ(&document, view->getDocument());
}
