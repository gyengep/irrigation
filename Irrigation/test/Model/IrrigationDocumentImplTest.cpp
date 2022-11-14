#include "IrrigationDocumentImplTest.h"
#include "DTO/DocumentDTO.h"
#include "Exceptions/Exceptions.h"
#include "Hardware/Valves/ZoneHandler.h"
#include "Logic/Program.h"
#include "Logic/RunTime.h"
#include "Logic/StartTime.h"
#include "Logic/ProgramContainer.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTimeContainer.h"
#include "Logic/WateringController.h"
#include "Schedulers/WeeklyScheduler.h"
#include "Mocks/MockDtoReader.h"
#include "Mocks/MockFileReader.h"
#include "Mocks/MockProgram.h"
#include "Dto2Object/DocumentSamples.h"
#include <gmock/gmock.h>
#include <cstdio>
#include <thread>

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;

///////////////////////////////////////////////////////////////////////////////
/*
const DocumentDTO expectedDocumentDTO(list<ProgramDTO>({
	ProgramDTO(true, "Abcdefg", 56, "weekly",
		EveryDaySchedulerDTO(),
		HotWeatherSchedulerDTO(65, 31),
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
*/
void IrrigationDocumentImplTest::SetUp() {
	irrigationDocument = IrrigationDocumentImpl::Builder().build();
}

void IrrigationDocumentImplTest::TearDown() {
}

void IrrigationDocumentImplTest::waitAndUnlock(IrrigationDocumentImpl* irrigationDocument, unsigned waitMs) {
	this_thread::sleep_for(chrono::milliseconds(waitMs + 10));
	irrigationDocument->unlock();
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(IrrigationDocumentImplTest, lockUnlock) {
	const unsigned waitMs = 100;

	irrigationDocument->lock();

	auto start = chrono::high_resolution_clock::now();
	thread lockAndUnlockThread(&IrrigationDocumentImplTest::waitAndUnlock, this, irrigationDocument.get(), waitMs);

	irrigationDocument->lock();
	irrigationDocument->unlock();
	auto end = chrono::high_resolution_clock::now();

	EXPECT_THAT(chrono::duration_cast<chrono::milliseconds>(end - start).count(), Ge(waitMs));

	lockAndUnlockThread.join();
}

///////////////////////////////////////////////////////////////////////////////
/*
void testToDocumentDto(const DocumentSample& documentSample) {
	EXPECT_THAT(documentSample.getObjectPtr()->toDocumentDto(), Eq(documentSample.getDto()));
}

TEST_F(IrrigationDocumentImplTest, toDocumentDto1) {
	testToDocumentDto(DocumentSample1());
}

TEST_F(IrrigationDocumentImplTest, toDocumentDto2) {
	testToDocumentDto(DocumentSample2());
}

TEST_F(IrrigationDocumentImplTest, toDocumentDto3) {
	testToDocumentDto(DocumentSample3());
}

TEST_F(IrrigationDocumentImplTest, toDocumentDto4) {
	testToDocumentDto(DocumentSample4());
}

///////////////////////////////////////////////////////////////////////////////

void testUpdateFromDocumentDto(const shared_ptr<IrrigationDocumentImpl>& irrigationDocument, const DocumentSample& documentSample) {
	EXPECT_THAT(irrigationDocument->getProgramContainer(), Not(Eq(std::ref(documentSample.getObjectPtr()->getPrograms()))));
	irrigationDocument->updateFromDocumentDto(Program, documentSample.getDto());
	EXPECT_THAT(irrigationDocument->getProgramContainer(), Eq(std::ref(documentSample.getObjectPtr()->getPrograms())));
}

TEST_F(IrrigationDocumentImplTest, updateFromDocumentDto1) {
	shared_ptr<IrrigationDocumentImpl> irrigationDocument = DocumentSample4().getObjectPtr();
	testUpdateFromDocumentDto(irrigationDocument, DocumentSample1());
}

TEST_F(IrrigationDocumentImplTest, updateFromDocumentDto2) {
	shared_ptr<IrrigationDocumentImpl> irrigationDocument = DocumentSample1().getObjectPtr();
	testUpdateFromDocumentDto(irrigationDocument, DocumentSample2());
}

TEST_F(IrrigationDocumentImplTest, updateFromDocumentDto3) {
	shared_ptr<IrrigationDocumentImpl> irrigationDocument = DocumentSample2().getObjectPtr();
	testUpdateFromDocumentDto(irrigationDocument, DocumentSample3());
}

TEST_F(IrrigationDocumentImplTest, updateFromDocumentDto4) {
	shared_ptr<IrrigationDocumentImpl> irrigationDocument = IrrigationDocumentImpl::Builder().build();
	testUpdateFromDocumentDto(irrigationDocument, DocumentSample4());
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(IrrigationDocumentImplTest, partialUpdateFromDocumentDto_empty) {
	const DocumentSample3 expected;

	DocumentSample3 actual;
	actual.getObjectPtr()->updateFromDocumentDto(DocumentDTO());
	EXPECT_THAT(actual.getObjectPtr()->getPrograms(), Eq(std::ref(expected.getObjectPtr()->getPrograms())));
}
*/
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
