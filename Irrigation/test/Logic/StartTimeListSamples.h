#pragma once
#include <list>
#include <memory>
#include "Logic/StartTime.h"
#include "Logic/StartTimeContainer.h"
#include "StartTimeSamples.h"


namespace LogicTest {

	class StartTimeListSample {
	protected:
		std::shared_ptr<StartTimeContainer> startTimeContainer;
		std::list<StartTimeDTO> startTimeDtoList;

	public:
		StartTimeListSample() : startTimeContainer(new StartTimeContainer()) {}
		virtual ~StartTimeListSample() = default;

		std::shared_ptr<StartTimeContainer> getStartTimeContainer() { return startTimeContainer; }
		const std::shared_ptr<StartTimeContainer> getStartTimeContainer() const { return startTimeContainer; }

		std::list<StartTimeDTO>& getStartTimeDtoList() { return startTimeDtoList; }
		const std::list<StartTimeDTO>& getStartTimeDtoList() const { return startTimeDtoList; }
	};

	class StartTimeListSampleEmpty : public StartTimeListSample {
	public:
		StartTimeListSampleEmpty() {}
	};

	class StartTimeListSampleOneItem : public StartTimeListSample {
	public:
		StartTimeListSampleOneItem() {
			startTimeContainer->insert(10, StartTimeSample1().getStartTime());
			startTimeDtoList.push_back(StartTimeSample1().getStartTimeDto().setId(10));
		}
	};

	class StartTimeListSampleMoreItem : public StartTimeListSample {
	public:
		StartTimeListSampleMoreItem() {
			startTimeContainer->insert(100, StartTimeSample1().getStartTime());
			startTimeContainer->insert(101, StartTimeSample2().getStartTime());
			startTimeContainer->insert(102, StartTimeSample3().getStartTime());
			startTimeContainer->insert(103, StartTimeSample4().getStartTime());

			startTimeDtoList.push_back(StartTimeSample1().getStartTimeDto().setId(100));
			startTimeDtoList.push_back(StartTimeSample2().getStartTimeDto().setId(101));
			startTimeDtoList.push_back(StartTimeSample3().getStartTimeDto().setId(102));
			startTimeDtoList.push_back(StartTimeSample4().getStartTimeDto().setId(103));
		}
	};
};
