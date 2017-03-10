
#ifndef IRRIGATION_WEBSERVER_H_
#define IRRIGATION_WEBSERVER_H_

#include <string>

#include "WebServer/WebServer.h"


class Document;
namespace Teng {
class FragmentList_t;
} /* namespace Teng */



class IrrigationWebServer : public WebServer {

	//typedef bool (*pGetFileFunctionPtr_t)( WebServer*, const Parameters&, const Parameters&, Answer& );
	//typedef std::map<std::string, pGetFileFunctionPtr_t> GetFileMap_t;
	//GetFileMap_t			m_theGetFileMap;

	const std::string rootDirectory;
	const Document* document;
/*
	void updateTemplateEngine();

	void addPrograms(const ProgramContainer& programs, Teng::FragmentList_t& programsFragment);
	void addRunTimes(const RunTimeContainer& runTime, Teng::FragmentList_t& runTimesFragment);
	void addStartTimes(const StartTimeContainer& startTime, Teng::FragmentList_t& startTimesFragment);
*/
public:
	IrrigationWebServer(const char* rootDirectory, unsigned short port, const Document* document);
	virtual ~IrrigationWebServer();

	virtual void onRequest(const Request& request);

/*
	bool GetFile_DefaultFile( const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer );
	static bool stat_GetFile_DefaultFile( WebServer* pThis, const Parameters& rGetParams, const Parameters& rPostParams, Answer& rAnswer ) { return pThis->GetFile_DefaultFile( rGetParams, rPostParams, rAnswer ); }
*/
};

#endif /* IRRIGATION_WEBSERVER_H_ */
