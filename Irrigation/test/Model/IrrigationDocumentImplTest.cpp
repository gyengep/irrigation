#include "IrrigationDocumentImplTest.h"
#include "Mocks/MockProgramContainer.h"
#include "Mocks/MockWateringController.h"
#include <thread>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void IrrigationDocumentImplTest::SetUp() {
	mockIrrigationDocumentLoader = std::make_shared<StrictMock<MockIrrigationDocumentLoader>>(),
	mockIrrigationDocumentSaver = std::make_shared<StrictMock<MockIrrigationDocumentSaver>>();

	irrigationDocument = std::make_shared<IrrigationDocumentImpl>(
			mockIrrigationDocumentLoader,
			mockIrrigationDocumentSaver,
			std::make_shared<StrictMock<MockProgramContainer>>(),
			std::make_shared<StrictMock<MockWateringController>>(),
			nullptr
		);
}

void IrrigationDocumentImplTest::TearDown() {
}

void IrrigationDocumentImplTest::waitAndUnlock(IrrigationDocumentImpl* irrigationDocument, unsigned waitMs) {
	std::this_thread::sleep_for(std::chrono::milliseconds(waitMs + 10));
	irrigationDocument->unlock();
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(IrrigationDocumentImplTest, lockUnlock) {
	const unsigned waitMs = 100;

	irrigationDocument->lock();

	auto start = std::chrono::high_resolution_clock::now();
	std::thread lockAndUnlockThread(&IrrigationDocumentImplTest::waitAndUnlock, this, irrigationDocument.get(), waitMs);

	irrigationDocument->lock();
	irrigationDocument->unlock();
	auto end = std::chrono::high_resolution_clock::now();

	EXPECT_THAT(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(), Ge(waitMs));

	lockAndUnlockThread.join();
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(IrrigationDocumentImplTest, isModified) {
	EXPECT_FALSE(irrigationDocument->isModified());
}

TEST_F(IrrigationDocumentImplTest, setModified) {
	irrigationDocument->setModified();
	EXPECT_TRUE(irrigationDocument->isModified());

	irrigationDocument->setModified(false);
	EXPECT_FALSE(irrigationDocument->isModified());

	irrigationDocument->setModified(true);
	EXPECT_TRUE(irrigationDocument->isModified());
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(IrrigationDocumentImplTest, load) {
	EXPECT_CALL(*mockIrrigationDocumentLoader, load(Ref(*irrigationDocument))).
			Times(1);

	irrigationDocument->load();
}

TEST_F(IrrigationDocumentImplTest, loadWithTrue) {
	EXPECT_CALL(*mockIrrigationDocumentLoader, load(Ref(*irrigationDocument))).
			Times(1).
			WillRepeatedly(Return(true));

	EXPECT_NO_THROW(irrigationDocument->load());
	EXPECT_FALSE(irrigationDocument->isModified());
}

TEST_F(IrrigationDocumentImplTest, loadWithFalse) {
	EXPECT_CALL(*mockIrrigationDocumentLoader, load(Ref(*irrigationDocument))).
			Times(1).
			WillRepeatedly(Return(false));

	EXPECT_NO_THROW(irrigationDocument->load());
	EXPECT_TRUE(irrigationDocument->isModified());
}

TEST_F(IrrigationDocumentImplTest, loadWithExceptiom) {
	EXPECT_CALL(*mockIrrigationDocumentLoader, load(Ref(*irrigationDocument))).
			Times(1).
			WillRepeatedly(Throw(std::runtime_error("aaa")));

	EXPECT_THROW(irrigationDocument->load(), std::runtime_error);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(IrrigationDocumentImplTest, save) {
	EXPECT_CALL(*mockIrrigationDocumentSaver, save(Ref(*irrigationDocument))).
			Times(1);

	irrigationDocument->save();
}

TEST_F(IrrigationDocumentImplTest, saveWithExceptiom) {
	EXPECT_CALL(*mockIrrigationDocumentSaver, save(Ref(*irrigationDocument))).
			Times(1).
			WillRepeatedly(Throw(std::runtime_error("aaa")));

	EXPECT_THROW(irrigationDocument->save(), std::runtime_error);
}
