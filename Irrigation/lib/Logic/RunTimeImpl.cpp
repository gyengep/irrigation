#include "Impl/RunTimeImpl.h"
#include "Exceptions/Exceptions.h"
#include <iomanip>
#include <sstream>

///////////////////////////////////////////////////////////////////////////////

RunTimePtr RunTimeImplFactory::create() const {
	return std::make_shared<RunTimeImpl>();
}

///////////////////////////////////////////////////////////////////////////////

const std::chrono::hours RunTimeImpl::maxTime(24);

RunTimeImpl::RunTimeImpl() :
	RunTimeImpl(std::chrono::seconds(0))
{
}

RunTimeImpl::RunTimeImpl(const std::chrono::seconds& time) {
	set(time);
}

std::chrono::seconds RunTimeImpl::get() const {
	return time;
}

void RunTimeImpl::set(const std::chrono::seconds& time) {
	if (time > maxTime) {
		throw ValueOutOfBoundsException(
				"RunTimeImpl value shall not be greater than " + std::to_string(maxTime.count()) +
				", while actual value is " + std::to_string(time.count()));
	}

	this->time = time;
}

std::chrono::seconds RunTimeImpl::toDuration() const {
	return time;
}

RunTimeDTO RunTimeImpl::toRunTimeDto() const {
	const unsigned seconds = std::chrono::duration_cast<std::chrono::seconds>(get()).count();
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

		set(std::chrono::minutes(minutes) + std::chrono::seconds(seconds));
	}
}

std::string RunTimeImpl::toString() const {
	const unsigned seconds = std::chrono::duration_cast<std::chrono::seconds>(get()).count();

	std::ostringstream oss;
	oss << std::setfill('0') << std::setw(2) << (seconds / 60) << ":";
	oss << std::setfill('0') << std::setw(2) << (seconds % 60);
	return oss.str();
}
