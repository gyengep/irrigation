#include <gmock/gmock.h>
#include "IrrigationDocumentTest.h"
#include "DTO/DocumentDTO.h"
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



void IrrigationDocumentTest::SetUp() {
	shared_ptr<GpioHandler> gpioHandler;
	shared_ptr<Valves> valves(new Valves(gpioHandler));
	shared_ptr<ZoneHandler> zoneHandler(new ZoneHandler(valves));
	document.reset(new IrrigationDocument(zoneHandler));
}

void IrrigationDocumentTest::TearDown() {
}


TEST_F(IrrigationDocumentTest, convertDocumentDTO) {
	const DocumentDTO expectedDocumentDTO(new list<ProgramDTO>({
		ProgramDTO("Abcdefg", "specified",
			SpecifiedSchedulerDTO(new list<bool>({ false, true, false, false, false, true, false})),
			new list<RunTimeDTO>({
				RunTimeDTO(20).setId(0),
				RunTimeDTO(21).setId(1),
				RunTimeDTO(22).setId(2),
				RunTimeDTO(23).setId(3),
				RunTimeDTO(24).setId(4),
				RunTimeDTO(25).setId(5)
			}),
			new list<StartTimeDTO>({
				StartTimeDTO(3600).setId(100),
				StartTimeDTO(3601).setId(101),
				StartTimeDTO(3602).setId(102)
			})
		).setId(15),
		ProgramDTO("Program2", "specified",
			SpecifiedSchedulerDTO(new list<bool>({ true, false, false, false, false, false, false})),
			new list<RunTimeDTO>({
				RunTimeDTO(120).setId(0),
				RunTimeDTO(121).setId(1),
				RunTimeDTO(122).setId(2),
				RunTimeDTO(123).setId(3),
				RunTimeDTO(124).setId(4),
				RunTimeDTO(125).setId(5)
			}),
			new list<StartTimeDTO>({
				StartTimeDTO(7200).setId(110),
				StartTimeDTO(7201).setId(111),
				StartTimeDTO(7202).setId(112)
			})
		).setId(25),
	}));

	document->updateFromDTO(expectedDocumentDTO);

	EXPECT_THAT(document->getPrograms().size(), Eq(2));
	EXPECT_THAT(document->getPrograms().at(15)->getProgramDTO().setId(15), *next(expectedDocumentDTO.getPrograms().begin(), 0));
	EXPECT_THAT(document->getPrograms().at(25)->getProgramDTO().setId(25), *next(expectedDocumentDTO.getPrograms().begin(), 1));
	EXPECT_EQ(expectedDocumentDTO, document->getDocumentDTO());
}
