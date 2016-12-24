
#include "Common.h"
#include "IrrigationWebServer.h"

#include "WebServer/Answer.h"
#include "WebServer/Request.h"


IrrigationWebServer::IrrigationWebServer(const char* rootDirectory, unsigned short port, const Document* document) :
	WebServer(port),
	rootDirectory(rootDirectory),
	document(document)
{
//	m_theGetFileMap.insert(std::make_pair(std::string("/"), stat_GetFile_DefaultFile));
}

IrrigationWebServer::~IrrigationWebServer() {
}
/*

void IrrigationWebServer::addPrograms(const ProgramContainer& programs, Teng::FragmentList_t& programsFragment) {
    for (auto it = programs.container().begin(); programs.container().end() != it; ++it) {
        const Program* program = it->second;
    	Teng::Fragment_t& programFragment = programsFragment.addFragment();

    	programFragment.addVariable("ID", (long)it->first);
    	programFragment.addVariable("name", program->getName());
    	addRunTimes(program->runTimes(), programFragment.addFragmentList("runTimes"));
    	addStartTimes(program->startTimes(), programFragment.addFragmentList("startTimes"));
    }
}

void IrrigationWebServer::addRunTimes(const RunTimeContainer& runTimes, Teng::FragmentList_t& runTimesFragment) {
    for (auto it = runTimes.container().begin(); runTimes.container().end() != it; ++it) {
    	Teng::Fragment_t& runTimeFragment = runTimesFragment.addFragment();

    	runTimeFragment.addVariable("ID", (long)it->first);
    	runTimeFragment.addVariable("minutes", (long)it->second);
    }
}

void IrrigationWebServer::addStartTimes(const StartTimeContainer& startTimes, Teng::FragmentList_t& startTimesFragment) {
    for (auto it = startTimes.container().begin(); startTimes.container().end() != it; ++it) {
    	Teng::Fragment_t& startTimeFragment = startTimesFragment.addFragment();

    	startTimeFragment.addVariable("ID", (long)it->first);
    	startTimeFragment.addVariable("minutes", (long)it->second);
    }
}

void IrrigationWebServer::updateTemplateEngine() {
    // Create Teng engine
    Teng::Teng_t teng("", Teng::Teng_t::LM_LOG_TO_OUTPUT);

    // Root data fragment
    Teng::Fragment_t root;

    addPrograms(document->programs(), root.addFragmentList("programs"));
}
*/
void IrrigationWebServer::onRequest(const Request& request) {
	static int a = 0;
	std::string text = std::to_string(a++);

	Answer answer(request, MHD_HTTP_OK, (void*)text.c_str(), text.length());
	send(answer);


	LOGGER.info("connection: %p", request.getConnection());
	LOGGER.info("url: %s", request.getUrl());
	LOGGER.info("method: %s", request.getMethod());
	LOGGER.info("version: %s", request.getVersion());
	LOGGER.info("size: %u", *request.getUploadDataSize());

/*
	std::string strFullPath = rootDirectory + fileName;

	LOGGER.info("WebServer: file requested: %s", strFullPath.c_str());

	FILE* f = fopen(strFullPath.c_str(), "rb");
	if (NULL == f) {
		return false;
	}

	long messageLength;
	char* messageBody;

	fseek(f, 0, SEEK_END);
	messageLength = ftell(f);
	messageBody = new char[messageLength];
	rewind(f);
	fread(messageBody, 1, messageLength, f);
	fclose(f);

	answer.replace(messageBody, messageLength);

	return true;
*/
/*
	GetFileMap_t::iterator it = m_theGetFileMap.find(fileName);
	bool fileFound;

	if (m_theGetFileMap.end() != it) {
		fileFound = it->second(this, getParameters, postParameters, answer);
	} else {
		fileFound = readFileFromDisk(fileName, answer);
	}

	return fileFound;
*/

}


