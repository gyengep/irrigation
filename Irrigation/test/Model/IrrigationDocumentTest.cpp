#include <gmock/gmock.h>
#include <cstdio>
#include <thread>
#include "IrrigationDocumentTest.h"
#include "DTO/DocumentDTO.h"
#include "Exceptions/Exceptions.h"
#include "Hardware/GpioHandler.h"
#include "Hardware/Valves.h"
#include "Hardware/ZoneHandler.h"
#include "Logic/Program.h"
#include "Logic/RunTime.h"
#include "Logic/StartTime.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTimeContainer.h"
#include "Schedulers/SpecifiedScheduler.h"

using namespace std;
using namespace testing;



const DocumentDTO expectedDocumentDTO(new list<ProgramDTO>({
	ProgramDTO("Abcdefg", "specified",
		SpecifiedSchedulerDTO(39, new list<bool>({ false, true, false, false, false, true, false})),
		new list<RunTimeDTO>({
			RunTimeDTO(2, 0).setId(0),
			RunTimeDTO(2, 1).setId(1),
			RunTimeDTO(2, 2).setId(2),
			RunTimeDTO(2, 3).setId(3),
			RunTimeDTO(2, 4).setId(4),
			RunTimeDTO(2, 5).setId(5)
		}),
		new list<StartTimeDTO>({
			StartTimeDTO(3, 20).setId(100),
			StartTimeDTO(4, 21).setId(101),
			StartTimeDTO(5, 22).setId(102)
		})
	).setId(15),
	ProgramDTO("Program2", "specified",
		SpecifiedSchedulerDTO(40, new list<bool>({ true, false, false, false, false, false, false})),
		new list<RunTimeDTO>({
			RunTimeDTO(1, 20).setId(0),
			RunTimeDTO(1, 21).setId(1),
			RunTimeDTO(1, 22).setId(2),
			RunTimeDTO(1, 23).setId(3),
			RunTimeDTO(1, 24).setId(4),
			RunTimeDTO(1, 25).setId(5)
		}),
		new list<StartTimeDTO>({
			StartTimeDTO(7, 20).setId(110),
			StartTimeDTO(7, 21).setId(111),
			StartTimeDTO(7, 22).setId(112)
		})
	).setId(25),
}));

void IrrigationDocumentTest::SetUp() {
	document.reset(new IrrigationDocument());
}

void IrrigationDocumentTest::TearDown() {
}

void IrrigationDocumentTest::waitAndUnlock(IrrigationDocument* document, unsigned waitMs) {
	this_thread::sleep_for(chrono::milliseconds(waitMs + 10));
	document->unlock();
}

TEST_F(IrrigationDocumentTest, lockUnlock) {
	const unsigned waitMs = 100;

	document->lock();

	auto start = chrono::high_resolution_clock::now();
	thread lockAndUnlockThread(&IrrigationDocumentTest::waitAndUnlock, this, document.get(), waitMs);

	document->lock();
	document->unlock();
	auto end = chrono::high_resolution_clock::now();

	EXPECT_THAT(chrono::duration_cast<chrono::milliseconds>(end - start).count(), Ge(waitMs));

	lockAndUnlockThread.join();
}

TEST_F(IrrigationDocumentTest, convertDocumentDTO) {
	document->updateFromDTO(expectedDocumentDTO);

	EXPECT_THAT(document->getPrograms().size(), Eq(2));
	EXPECT_THAT(document->getPrograms().at(15)->getProgramDTO().setId(15), *next(expectedDocumentDTO.getPrograms().begin(), 0));
	EXPECT_THAT(document->getPrograms().at(25)->getProgramDTO().setId(25), *next(expectedDocumentDTO.getPrograms().begin(), 1));
	EXPECT_THAT(document->getDocumentDTO(), Eq(expectedDocumentDTO));
}

TEST_F(IrrigationDocumentTest, loadInvalidFile) {
	EXPECT_THROW(document->load(tmpnam(nullptr)), FileNotFoundException);
}

TEST_F(IrrigationDocumentTest, load) {
	MockDtoReader* dtoReader = new MockDtoReader();
	MockFileReader* fileReader = new MockFileReader();

	document.reset(new IrrigationDocument(
			new MockDtoReaderWriterFactory(dtoReader),
			new MockFileReaderWriterFactory(fileReader)
			));

	const string fileName("12345678");
	const string text("abcdefg");

	EXPECT_CALL(*fileReader, read(fileName))
			.Times(1)
			.WillOnce(Return(text));

	EXPECT_CALL(*dtoReader, loadDocument(text))
			.Times(1)
			.WillOnce(Return(expectedDocumentDTO));

	EXPECT_NO_THROW(document->load(fileName));
	EXPECT_THAT(document->getDocumentDTO(), Eq(expectedDocumentDTO));
}
