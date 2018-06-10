#include <gmock/gmock.h>
#include "Model/IrrigationDocument.h"
#include "DTO/DocumentDTO.h"
#include "Logic/Program.h"
#include "Logic/RunTime.h"
#include "Logic/StartTime.h"
#include "Logic/RunTimeContainer.h"
#include "Logic/StartTimeContainer.h"
#include "Schedulers/SpecifiedScheduler.h"

using namespace std;



TEST(IrrigationDocument, getDocumentDTO) {
	IrrigationDocument document;

	document.getPrograms().insert(0, new Program());
	document.getPrograms().at(0)->setName("Abcdefg");
	document.getPrograms().at(0)->setSchedulerType(SchedulerType::SPECIFIED_DAYS);
	document.getPrograms().at(0)->getSpecifiedScheduler().enableDay(SpecifiedScheduler::MONDAY, true);
	document.getPrograms().at(0)->getSpecifiedScheduler().enableDay(SpecifiedScheduler::FRIDAY, true);
	document.getPrograms().at(0)->getRunTimes().at(0)->setValue(20);
	document.getPrograms().at(0)->getRunTimes().at(1)->setValue(21);
	document.getPrograms().at(0)->getRunTimes().at(2)->setValue(22);
	document.getPrograms().at(0)->getRunTimes().at(3)->setValue(23);
	document.getPrograms().at(0)->getRunTimes().at(4)->setValue(24);
	document.getPrograms().at(0)->getRunTimes().at(5)->setValue(25);
	document.getPrograms().at(0)->getStartTimes().insert(100, new StartTime(1, 0, 0));
	document.getPrograms().at(0)->getStartTimes().insert(101, new StartTime(1, 0, 1));
	document.getPrograms().at(0)->getStartTimes().insert(102, new StartTime(1, 0, 2));

	document.getPrograms().insert(1, new Program());
	document.getPrograms().at(1)->setName("Program2");
	document.getPrograms().at(1)->setSchedulerType(SchedulerType::SPECIFIED_DAYS);
	document.getPrograms().at(1)->getSpecifiedScheduler().enableDay(SpecifiedScheduler::SUNDAY, true);
	document.getPrograms().at(1)->getRunTimes().at(0)->setValue(120);
	document.getPrograms().at(1)->getRunTimes().at(1)->setValue(121);
	document.getPrograms().at(1)->getRunTimes().at(2)->setValue(122);
	document.getPrograms().at(1)->getRunTimes().at(3)->setValue(123);
	document.getPrograms().at(1)->getRunTimes().at(4)->setValue(124);
	document.getPrograms().at(1)->getRunTimes().at(5)->setValue(125);
	document.getPrograms().at(1)->getStartTimes().insert(110, new StartTime(2, 0, 0));
	document.getPrograms().at(1)->getStartTimes().insert(111, new StartTime(2, 0, 1));
	document.getPrograms().at(1)->getStartTimes().insert(112, new StartTime(2, 0, 2));


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
		).setId(0),
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
		).setId(1),
	}));

	EXPECT_EQ(expectedDocumentDTO, document.getDocumentDTO());
}
