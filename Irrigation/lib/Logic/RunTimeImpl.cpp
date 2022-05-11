#include "RunTimeImpl.h"
#include "Exceptions/Exceptions.h"

///////////////////////////////////////////////////////////////////////////////

RunTimePtr RunTimeFactory::create() const {
	return std::make_shared<RunTimeImpl>();
}

///////////////////////////////////////////////////////////////////////////////

RunTimeImpl::RunTimeImpl() : RunTimeImpl(0) {
}

RunTimeImpl::RunTimeImpl(unsigned seconds) :
	milliSeconds(seconds * 1000)
{
}

RunTimeImpl::RunTimeImpl(const std::chrono::seconds& seconds) : RunTimeImpl(seconds.count()) {
}

unsigned RunTimeImpl::getSeconds() const {
	return milliSeconds / 1000;
}

unsigned RunTimeImpl::getMilliSeconds() const {
	return milliSeconds;
}

void RunTimeImpl::setSeconds(unsigned seconds) {
	if (seconds > maxSeconds) {
		throw ValueOutOfBoundsException(
				"RunTimeImpl value shall not be greater than " + std::to_string(maxSeconds) +
				", while actual value is " + std::to_string(seconds));
	}

	this->milliSeconds = seconds * 1000;
}

void RunTimeImpl::setMilliSeconds(unsigned milliSeconds) {
	if (milliSeconds > maxSeconds * 1000) {
		throw ValueOutOfBoundsException(
				"RunTimeImpl value shall not be greater than " + std::to_string(maxSeconds * 1000) + " ms" +
				", while actual value is " + std::to_string(milliSeconds));
	}

	this->milliSeconds = milliSeconds;
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
