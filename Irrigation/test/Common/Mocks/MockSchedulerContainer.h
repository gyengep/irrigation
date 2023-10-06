#pragma once
#include <gmock/gmock.h>
#include "Logic/SchedulerContainer.h"


class MockSchedulerContainer : public SchedulerContainer {
public:
	MOCK_CONST_METHOD0(begin, const_iterator());
	MOCK_CONST_METHOD0(end, const_iterator());
	MOCK_CONST_METHOD0(size, size_t());

	MOCK_CONST_METHOD1(at, const_mapped_type(const key_type& key));
	MOCK_METHOD1(at, mapped_type(const key_type& key));

	MOCK_CONST_METHOD0(getEveryDayScheduler, const EveryDayScheduler&());
	MOCK_CONST_METHOD0(getHotWeatherScheduler, const HotWeatherScheduler&());
	MOCK_CONST_METHOD0(getTemperatureDependentScheduler, const TemperatureDependentScheduler&());
	MOCK_CONST_METHOD0(getWeeklyScheduler, const WeeklyScheduler&());

	MOCK_METHOD0(getEveryDayScheduler, EveryDayScheduler&());
	MOCK_METHOD0(getHotWeatherScheduler, HotWeatherScheduler&());
	MOCK_METHOD0(getTemperatureDependentScheduler, TemperatureDependentScheduler&());
	MOCK_METHOD0(getWeeklyScheduler, WeeklyScheduler&());

	MOCK_METHOD0(createPersistedData, void());
	MOCK_METHOD0(deletePersistedData, void());
	MOCK_METHOD0(loadPersistedData, void());

	MOCK_CONST_METHOD0(toSchedulersDto, SchedulersDTO());
	MOCK_METHOD1(updateFromSchedulersDto, void(const SchedulersDTO& schedulerdDto));

	MOCK_CONST_METHOD0(toString, std::string());
};

///////////////////////////////////////////////////////////////////////////////

class MockSchedulerContainerFactory : public SchedulerContainerFactory {
public:
	virtual ~MockSchedulerContainerFactory() = default;
	virtual SchedulerContainerPtr create(unsigned) const override {
		return std::make_shared<MockSchedulerContainer>();
	}
};
