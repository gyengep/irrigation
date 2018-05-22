#include <gmock/gmock.h>
#include "DocumentView/Document.h"
#include "DocumentView/View.h"



class MockView : public View {
public:

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
	EXPECT_CALL(*view, destructorIsCalled());

	document.addView(view);
}

TEST(DocumentTest, removeView) {
	Document document;
	MockView* view = new MockView();

	EXPECT_CALL(*view, init());
	EXPECT_CALL(*view, terminate()).Times(1);
	EXPECT_CALL(*view, destructorIsCalled()).Times(1);

	document.addView(view);
	document.removeView(view);
}

TEST(DocumentTest, documentPtrSet) {
	Document document;
	MockView* view = new MockView();

	EXPECT_CALL(*view, init());
	EXPECT_CALL(*view, terminate());
	EXPECT_CALL(*view, destructorIsCalled());

	document.addView(view);

	EXPECT_EQ(&document, view->getDocument());
}
