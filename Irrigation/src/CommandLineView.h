/*
 * CommandLineView.h
 *
 *  Created on: 2016. máj. 15.
 *      Author: Rendszergazda
 */

#ifndef COMMANDLINEVIEW_H_
#define COMMANDLINEVIEW_H_

#include <atomic>
#include <string>
#include <thread>

#include "CommandExecutor.h"
#include "View.h"


class CommandLineView : public CommandExecutor, View, CommandCallback {
	std::thread* workerThread;
	std::atomic_bool isTerminated;

	void workerFunc();

	virtual void onHelpSuccess();
	virtual void onStarttimeListSuccess(const StartTimeContainer& startTimes);
	virtual void onStarttimeSetSuccess();
	virtual void onStarttimeGetSuccess(IdType startTimeId, unsigned startTime);
	virtual void onStarttimeAddSuccess();
	virtual void onStarttimeDeleteSuccess();
	virtual void onRuntimeListSuccess(const RunTimeContainer& runTimes);
	virtual void onRuntimeSetSuccess();
	virtual void onRuntimeGetSuccess(IdType runTimeId, unsigned runTime);
	virtual void onProgramListSuccess(const Document::Programs& programs);
	virtual void onProgramShowSuccess(const Program& program);
	virtual void onProgramAddSuccess();
	virtual void onProgramDeleteSuccess();
	virtual void onProgramRenameSuccess();
	virtual void onProgramMoveSuccess();
	virtual void onValveSuccess();
	virtual void onZoneSuccess();
	virtual void onResetValvesSuccess();
	virtual void onExitSuccess();

public:
	CommandLineView(Document* document);
	virtual ~CommandLineView();

	static void tokenize(const std::string& text, Tokens& tokens);
};

#endif /* COMMANDLINEVIEW_H_ */
