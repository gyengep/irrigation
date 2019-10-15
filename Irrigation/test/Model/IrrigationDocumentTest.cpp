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
	ProgramDTO(true, "Abcdefg", 56, "weekly",
		EveryDaySchedulerDTO(),
		HotWeatherSchedulerDTO(65, 31),
		PeriodicSchedulerDTO(list<bool>({ false, true, false}), 2016, 1, 2),
		TemperatureDependentSchedulerDTO(10.01f, 50, 51, 52),
		WeeklySchedulerDTO(list<bool>({ false, true, false, false, false, true, false})),
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
	ProgramDTO(false, "Program2", 106, "weekly",
		EveryDaySchedulerDTO(),
		HotWeatherSchedulerDTO(70, 28),
		PeriodicSchedulerDTO(list<bool>({ true, false}), 2018, 10, 20),
		TemperatureDependentSchedulerDTO(20.01f, 30, 31, 32),
		WeeklySchedulerDTO(list<bool>({ true, false, false, false, false, false, false})),
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
	irrigationDocument = IrrigationDocument::Builder().build();
}

void IrrigationDocumentTest::TearDown() {
}

void IrrigationDocumentTest::waitAndUnlock(IrrigationDocument* irrigationDocument, unsigned waitMs) {
	this_thread::sleep_for(chrono::milliseconds(waitMs + 10));
	irrigationDocument->unlock();
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(IrrigationDocumentTest, lockUnlock) {
	const unsigned waitMs = 100;

	irrigationDocument->lock();

	auto start = chrono::high_resolution_clock::now();
	thread lockAndUnlockThread(&IrrigationDocumentTest::waitAndUnlock, this, irrigationDocument.get(), waitMs);

	irrigationDocument->lock();
	irrigationDocument->unlock();
	auto end = chrono::high_resolution_clock::now();

	EXPECT_THAT(chrono::duration_cast<chrono::milliseconds>(end - start).count(), Ge(waitMs));

	lockAndUnlockThread.join();
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

void testUpdateFromDocumentDto(shared_ptr<IrrigationDocument> irrigationDocument, const DocumentSample& documentSample) {
	EXPECT_THAT(irrigationDocument->getPrograms(), Not(documentSample.getObject()->getPrograms()));
	irrigationDocument->updateFromDocumentDto(documentSample.getDto());
	EXPECT_THAT(irrigationDocument->getPrograms(), documentSample.getObject()->getPrograms());
}

TEST_F(IrrigationDocumentTest, updateFromDocumentDto1) {
	shared_ptr<IrrigationDocument> irrigationDocument = DocumentSample4().getObject();
	testUpdateFromDocumentDto(irrigationDocument, DocumentSample1());
}

TEST_F(IrrigationDocumentTest, updateFromDocumentDto2) {
	shared_ptr<IrrigationDocument> irrigationDocument = DocumentSample1().getObject();
	testUpdateFromDocumentDto(irrigationDocument, DocumentSample2());
}

TEST_F(IrrigationDocumentTest, updateFromDocumentDto3) {
	shared_ptr<IrrigationDocument> irrigationDocument = DocumentSample2().getObject();
	testUpdateFromDocumentDto(irrigationDocument, DocumentSample3());
}

TEST_F(IrrigationDocumentTest, updateFromDocumentDto4) {
	shared_ptr<IrrigationDocument> irrigationDocument = IrrigationDocument::Builder().build();
	testUpdateFromDocumentDto(irrigationDocument, DocumentSample4());
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(IrrigationDocumentTest, partialUpdateFromDocumentDto_empty) {
	const DocumentSample3 expected;

	DocumentSample3 actual;
	actual.getObject()->updateFromDocumentDto(DocumentDTO());
	EXPECT_THAT(actual.getObject()->getPrograms(), Eq(expected.getObject()->getPrograms()));
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(IrrigationDocumentTest, isModified) {
	EXPECT_FALSE(irrigationDocument->isModified());
}

TEST_F(IrrigationDocumentTest, setModified) {
	irrigationDocument->setModified();
	EXPECT_TRUE(irrigationDocument->isModified());

	irrigationDocument->setModified(false);
	EXPECT_FALSE(irrigationDocument->isModified());

	irrigationDocument->setModified(true);
	EXPECT_TRUE(irrigationDocument->isModified());
}
