#include "IrrigationDocumentTest.h"
#include <gmock/gmock.h>
#include <cstdio>
#include <thread>
#include "DTO/DocumentDTO.h"
#include "Exceptions/Exceptions.h"
#include "Hardware/Valves/GpioHandler.h"
#include "Hardware/Valves/ZoneHandler.h"
#include "Logic/Program.h"
#include "Logic/RunTime.h"
#include "Logic/StartTime.h"
#include "Logic/ProgramContainer.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTimeContainer.h"
#include "Logic/WateringController.h"
#include "Model/IrrigationDocument.h"
#include "Schedulers/WeeklyScheduler.h"
#include "Mocks/MockDtoReader.h"
#include "Mocks/MockFileReader.h"
#include "Mocks/MockProgram.h"
#include "Dto2Object/DocumentSamples.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;

///////////////////////////////////////////////////////////////////////////////

const DocumentDTO expectedDocumentDTO(list<ProgramDTO>({
	ProgramDTO("Abcdefg", "weekly",
		PeriodicSchedulerDTO(56, list<bool>({ false, true, false}), 2016, 1, 2),
		WeeklySchedulerDTO(39, list<bool>({ false, true, false, false, false, true, false})),
		list<RunTimeDTO>({
			RunTimeDTO(2, 0).setId(0),
			RunTimeDTO(2, 1).setId(1),
			RunTimeDTO(2, 2).setId(2),
			RunTimeDTO(2, 3).setId(3),
			RunTimeDTO(2, 4).setId(4),
			RunTimeDTO(2, 5).setId(5)
		}),
		list<StartTimeDTO>({
			StartTimeDTO(3, 20).setId(100),
			StartTimeDTO(4, 21).setId(101),
			StartTimeDTO(5, 22).setId(102)
		})
	).setId(15),
	ProgramDTO("Program2", "weekly",
		PeriodicSchedulerDTO(106, list<bool>({ true, false}), 2018, 10, 20),
		WeeklySchedulerDTO(40, list<bool>({ true, false, false, false, false, false, false})),
		list<RunTimeDTO>({
			RunTimeDTO(1, 20).setId(0),
			RunTimeDTO(1, 21).setId(1),
			RunTimeDTO(1, 22).setId(2),
			RunTimeDTO(1, 23).setId(3),
			RunTimeDTO(1, 24).setId(4),
			RunTimeDTO(1, 25).setId(5)
		}),
		list<StartTimeDTO>({
			StartTimeDTO(7, 20).setId(110),
			StartTimeDTO(7, 21).setId(111),
			StartTimeDTO(7, 22).setId(112)
		})
	).setId(25),
}));

void IrrigationDocumentTest::SetUp() {
	document = IrrigationDocument::Builder().build();
}

void IrrigationDocumentTest::TearDown() {
}

void IrrigationDocumentTest::waitAndUnlock(IrrigationDocument* document, unsigned waitMs) {
	this_thread::sleep_for(chrono::milliseconds(waitMs + 10));
	document->unlock();
}

///////////////////////////////////////////////////////////////////////////////

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

TEST_F(IrrigationDocumentTest, loadInvalidFile) {
	EXPECT_THROW(document->load(tmpnam(nullptr)), FileNotFoundException);
}

TEST_F(IrrigationDocumentTest, load) {
	const string fileName("12345678");
	const string text("abcdefg");

	unique_ptr<MockDtoReader> dtoReader(new MockDtoReader());
	unique_ptr<MockFileReader> fileReader(new MockFileReader());

	EXPECT_CALL(*fileReader, read(fileName))
			.Times(1)
			.WillOnce(Return(text));

	EXPECT_CALL(*dtoReader, loadDocument(text))
			.Times(1)
			.WillOnce(Return(expectedDocumentDTO));

	document = IrrigationDocument::Builder()
			.setDtoReaderWriterFactory(unique_ptr<DtoReaderWriterFactory>(new MockDtoReaderWriterFactory(move(dtoReader))))
			.setFileReaderWriterFactory(unique_ptr<FileReaderWriterFactory>(new MockFileReaderWriterFactory(move(fileReader))))
			.build();

	EXPECT_NO_THROW(document->load(fileName));
	EXPECT_THAT(document->toDocumentDto(), Eq(expectedDocumentDTO));
}

///////////////////////////////////////////////////////////////////////////////

void testToDocumentDto(const DocumentSample& documentSample) {
	EXPECT_THAT(documentSample.getObject()->toDocumentDto(), Eq(documentSample.getDto()));
}

TEST_F(IrrigationDocumentTest, toDocumentDto1) {
	testToDocumentDto(DocumentSample1());
}

TEST_F(IrrigationDocumentTest, toDocumentDto2) {
	testToDocumentDto(DocumentSample2());
}

TEST_F(IrrigationDocumentTest, toDocumentDto3) {
	testToDocumentDto(DocumentSample3());
}

TEST_F(IrrigationDocumentTest, toDocumentDto4) {
	testToDocumentDto(DocumentSample4());
}

///////////////////////////////////////////////////////////////////////////////

void testUpdateFromDocumentDto(shared_ptr<IrrigationDocument> document, const DocumentSample& documentSample) {
	EXPECT_THAT(document->getPrograms(), Not(documentSample.getObject()->getPrograms()));
	document->updateFromDocumentDto(documentSample.getDto());
	EXPECT_THAT(document->getPrograms(), documentSample.getObject()->getPrograms());
}

TEST_F(IrrigationDocumentTest, updateFromDocumentDto1) {
	shared_ptr<IrrigationDocument> document = DocumentSample4().getObject();
	testUpdateFromDocumentDto(document, DocumentSample1());
}

TEST_F(IrrigationDocumentTest, updateFromDocumentDto2) {
	shared_ptr<IrrigationDocument> document = DocumentSample1().getObject();
	testUpdateFromDocumentDto(document, DocumentSample2());
}

TEST_F(IrrigationDocumentTest, updateFromDocumentDto3) {
	shared_ptr<IrrigationDocument> document = DocumentSample2().getObject();
	testUpdateFromDocumentDto(document, DocumentSample3());
}

TEST_F(IrrigationDocumentTest, updateFromDocumentDto4) {
	shared_ptr<IrrigationDocument> document = IrrigationDocument::Builder().build();
	testUpdateFromDocumentDto(document, DocumentSample4());
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(IrrigationDocumentTest, partialUpdateFromDocumentDto_empty) {
	const DocumentSample1 expected;

	DocumentSample1 actual;
	actual.getObject()->updateFromDocumentDto(DocumentDTO());
	EXPECT_THAT(actual.getObject()->getPrograms(), Eq(expected.getObject()->getPrograms()));
}
