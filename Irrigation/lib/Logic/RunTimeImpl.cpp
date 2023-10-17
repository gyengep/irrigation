#include "Impl/RunTimeImpl.h"
#include "Exceptions/Exceptions.h"
#include <iomanip>
#include <sstream>

///////////////////////////////////////////////////////////////////////////////

RunTimePtr RunTimeImplFactory::create() const {
	return std::make_shared<RunTimeImpl>();
}

///////////////////////////////////////////////////////////////////////////////

RunTimeImpl::RunTimeImpl() :
	RunTimeImpl(0)
{
}

RunTimeImpl::RunTimeImpl(const std::chrono::seconds& seconds) :
	RunTimeImpl(seconds.count())
{
}

RunTimeImpl::RunTimeImpl(unsigned seconds) :
	seconds(seconds)
{
}

unsigned RunTimeImpl::getSeconds() const {
	return seconds;
}

void RunTimeImpl::setSeconds(unsigned seconds) {
	if (seconds > maxSeconds) {
		throw ValueOutOfBoundsException(
				"RunTimeImpl value shall not be greater than " + std::to_string(maxSeconds) +
				", while actual value is " + std::to_string(seconds));
	}

	this->seconds = seconds;
}

std::chrono::seconds RunTimeImpl::toDuration() const {
	return std::chrono::seconds(seconds);
}

RunTimeDTO RunTimeImpl::toRunTimeDto() const {
	const unsigned seconds = getSeconds();
	return RunTimeDTO(seconds / 60, seconds % 60);
}

void RunTimeImpl::updateFromRunTimeDto(const RunTimeDTO& runTimeDTO) {
	if (runTimeDTO.hasMinutes() || runTimeDTO.hasSeconds()) {
		unsigned minutes = 0;
		unsigned seconds = 0;

		if (runTimeDTO.hasMinutes()) {
			minutes = runTimeDTO.getMinutes();
		}

		if (runTimeDTO.hasSeconds()) {
			seconds = runTimeDTO.getSeconds();
		}

		setSeconds(60 * minutes + seconds);
	}
}

std::string RunTimeImpl::toString() const {
	std::ostringstream oss;
	oss << std::setfill('0') << std::setw(2) << (getSeconds() / 60) << ":";
	oss << std::setfill('0') << std::setw(2) << (getSeconds() % 60);
	return oss.str();
}
