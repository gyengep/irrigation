#include "IrrigationDocumentImplTest.h"
#include <thread>

using namespace testing;

///////////////////////////////////////////////////////////////////////////////

void IrrigationDocumentImplTest::SetUp() {
	irrigationDocument = IrrigationDocumentImpl::Builder().build();
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
