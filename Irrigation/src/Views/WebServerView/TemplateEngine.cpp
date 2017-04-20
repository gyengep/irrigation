#include "common.h"
#include "TemplateEngine.h"

#include <tengwriter.h>
#include <tengerror.h>

#include "Model/IrrigationDocument.h"


using namespace Teng;


TemplateEngine::TemplateEngine(IrrigationDocument* const document) :
	document(document)
{
}

TemplateEngine::~TemplateEngine() {
}

const std::string& TemplateEngine::generate(const std::string& templateText, const std::map<std::string, std::string>& parameters) {

	LOGGER.debug("TemplateEngine generating page");

	generated.clear();

	try {
		Teng_t teng("", Teng_t::LM_LOG_TO_OUTPUT);

	    // Root data fragment
	    Fragment_t data;
	    addDatas(data);
	    addParameters(data, parameters);
/*
	    std::ostringstream o;
	    data.dump(o);
	    LOGGER.trace("template data: %s", o.str().c_str());
*/
	    // Output to standard output
	    StringWriter_t writer(generated);

	    // Simple error log
	    Error_t err;

	    // Generate page
	    int result = teng.generatePage(
	    	templateText, // Template
	        "", // Dictionary (none)
	        "", // Language (none)
	        "", // Configuration (none)
	        "text/html", // Content type
	        "utf-8", // Encoding
	        data, // Root fragment
	        writer, // Writer
	        err // Error log
	    );

	    if (0 != result) {
	    	LOGGER.debug("Template engine generate error: %d", result);
	    }
	} catch(std::exception& e) {
		LOGGER.warning("Template engine error: %s", e.what());
	} catch(...) {
		LOGGER.warning("Template engine unknown error");
	}

    return generated;
}

void TemplateEngine::addDatas(Fragment_t& root) {
    FragmentList_t &programList = root.addFragmentList("program");
	auto f = std::bind(&TemplateEngine::onIterate, this, &programList, std::placeholders::_1, std::placeholders::_2);

    document->getPrograms().iterate(f);
}

void TemplateEngine::onIterate(FragmentList_t* programList, IdType id, LockedProgram program) {
    Fragment_t& programFragment = programList->addFragment();

    programFragment.addVariable("id", std::to_string(id));
    programFragment.addVariable("name", program->getName());

    FragmentList_t& runTimeList = programFragment.addFragmentList("runTime");
    for (auto it = program->getRunTimes().begin(); it != program->getRunTimes().end(); ++it) {
    	Fragment_t& runTime = runTimeList.addFragment();

    	runTime.addVariable("id", std::to_string(it->first));
    	runTime.addVariable("minutes", std::to_string(it->second));
    }

    FragmentList_t& startTimeList = programFragment.addFragmentList("startTime");
    for (auto it = program->getStartTimes().begin(); it != program->getStartTimes().end(); ++it) {
    	Fragment_t& startTime = startTimeList.addFragment();

    	startTime.addVariable("id", std::to_string(it->first));
    	startTime.addVariable("hours", std::to_string(it->second.getHour()));
    	startTime.addVariable("minutes", std::to_string(it->second.getMin()));
    }
}

void TemplateEngine::addParameters(Teng::Fragment_t& root, const std::map<std::string, std::string>& parameters) {
    Fragment_t& parameterFragment = root.addFragment("parameter");

    for (auto it = parameters.begin(); parameters.end() != it; ++it) {
    	LOGGER.trace("template data: parameter.%s: \"%s\"", it->first.c_str(), it->second.c_str());
    	parameterFragment.addVariable(it->first, it->second);
	}
}
