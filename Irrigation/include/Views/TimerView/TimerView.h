#pragma once
#include "Timer.h"
#include "DocumentView/View.h"

class IrrigationDocument;


class TimerView : public View, public TimerCallback {
	IrrigationDocument& irrigationDocument;
	Timer timer;

public:
	TimerView(IrrigationDocument& irrigationDocument);
	virtual ~TimerView();

	virtual void initialize() override;
	virtual void terminate() override;

	virtual void onTimer(time_t rawTime);
};
