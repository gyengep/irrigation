
#ifndef IRRIGATION_WEBSERVER_H_
#define IRRIGATION_WEBSERVER_H_

#include <cstring>
#include <Teng.h>

#include "Model/Document.h"
#include "WebServer/WebServer.h"



class IrrigationWebServer : public WebServer {

	//typedef bool (*pGetFileFunctionPtr_t)( WebServer*, const Parameters&, const Parameters&, Answer& );
	//typedef std::map<std::string, pGetFileFunctionPtr_t> GetFileMap_t;
	//GetFileMap_t			m_theGetFileMap;

	const std::string rootDirectory;
	const Document* document;

	void updateTemplateEngine();

	void addPrograms(const ProgramContainer& programs, Teng::FragmentList_t& programsFragment);
	void addRunTimes(const RunTimeContainer& runTime, Teng::FragmentList_t& runTimesFragment);
	void addStartTimes(const StartTimeContainer& startTime, Teng::FragmentList_t& startTimesFragment);

public:
	IrrigationWebServer(const char* rootDirectory, unsigned short port, const Document* document);
	virtual ~IrrigationWebServer();

	virtual bool getFile(const std::string& fileName, const Parameters& getParameters, const Parameters& postParameters, Answer& answer);

/*
	bool GetFile_DefaultFile( const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer );
	static bool stat_GetFile_DefaultFile( WebServer* pThis, const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer ) { return pThis->GetFile_DefaultFile( rGetParams, rPostParams, rAnswer ); }
*/
};

#endif /* IRRIGATION_WEBSERVER_H_ */
