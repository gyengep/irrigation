#include "common.h"
#include "TemplateEngine.h"

#include <tengwriter.h>
#include <tengerror.h>

#include "Model/IrrigationDocument.h"


using namespace Teng;


TemplateEngine::TemplateEngine(IrrigationDocument* const document) :
	document(document),
	teng("", Teng_t::LM_LOG_TO_OUTPUT)
{
}

TemplateEngine::~TemplateEngine() {
}

const std::string& TemplateEngine::generate(const std::string& source) {

	LOGGER.trace("TamplateEngine generating page");

	result.clear();

    // Root data fragment
    Fragment_t root;
    initData(root);

    // Output to standard output
    StringWriter_t writer(result);

    // Simple error log
    Error_t err;

    // Generate page
    teng.generatePage(
    	source, // Template
        "", // Dictionary (none)
        "", // Language (none)
        "", // Configuration (none)
        "text/html", // Content type
        "utf-8", // Encoding
        root, // Root fragment
        writer, // Writer
        err // Error log
    );

    return result;
}

void TemplateEngine::initData(Fragment_t& root) {
    FragmentList_t &programList = root.addFragmentList("program");
	auto f = std::bind(&TemplateEngine::onIterate, this, &programList, std::placeholders::_1, std::placeholders::_2);

    document->getPrograms().iterate(f);
}

void TemplateEngine::onIterate(FragmentList_t* programList, IdType id, LockedProgram program) {
    Fragment_t &fragment = programList->addFragment();

    fragment.addVariable("id", std::to_string(id));
    fragment.addVariable("name", program->getName());

    FragmentList_t& runTimeList = fragment.addFragmentList("runTime");
    for (auto it = program->getRunTimes().begin(); it != program->getRunTimes().end(); ++it) {
    	Fragment_t& runTime = runTimeList.addFragment();

    	runTime.addVariable("id", std::to_string(it->first));
    	runTime.addVariable("minutes", std::to_string(it->second));
    }

    FragmentList_t& startTimeList = fragment.addFragmentList("startTime");
    for (auto it = program->getStartTimes().begin(); it != program->getStartTimes().end(); ++it) {
    	Fragment_t& startTime = startTimeList.addFragment();

    	startTime.addVariable("id", std::to_string(it->first));
    	startTime.addVariable("hours", std::to_string(it->second.getHour()));
    	startTime.addVariable("minutes", std::to_string(it->second.getMin()));
    }
}


