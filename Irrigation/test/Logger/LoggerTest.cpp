#include <gmock/gmock.h>
#include <fstream>
#include <iomanip>
#include <thread>
#include "Exceptions/Exceptions.h"
#include "Logger/Logger.h"

using namespace testing;


#define LOG_ENTRY_ERROR 	"log_entry_error"
#define LOG_ENTRY_WARNING 	"log_entry_warning"
#define LOG_ENTRY_INFO 		"log_entry_info"
#define LOG_ENTRY_DEBUG 	"log_entry_debug"
#define LOG_ENTRY_TRACE 	"log_entry_trace"

#define LOG_EXCEPTION_ERROR_MESSAGE 	"log_exception_error_message"
#define LOG_EXCEPTION_ERROR_INNER 		"log_exception_error_inner"
#define LOG_EXCEPTION_ERROR_OUTTER 		"log_exception_error_outter"
#define LOG_EXCEPTION_WARNING_MESSAGE 	"log_exception_warning_message"
#define LOG_EXCEPTION_WARNING_INNER 	"log_exception_warning_inner"
#define LOG_EXCEPTION_WARNING_OUTTER 	"log_exception_warning_outter"
#define LOG_EXCEPTION_INFO_MESSAGE 		"log_exception_info_message"
#define LOG_EXCEPTION_INFO_INNER 		"log_exception_info_inner"
#define LOG_EXCEPTION_INFO_OUTTER 		"log_exception_info_outter"
#define LOG_EXCEPTION_DEBUG_MESSAGE 	"log_exception_debug_message"
#define LOG_EXCEPTION_DEBUG_INNER 		"log_exception_debug_inner"
#define LOG_EXCEPTION_DEBUG_OUTTER 		"log_exception_debug_outter"
#define LOG_EXCEPTION_TRACE_MESSAGE 	"log_exception_trace_message"
#define LOG_EXCEPTION_TRACE_INNER 		"log_exception_trace_inner"
#define LOG_EXCEPTION_TRACE_OUTTER 		"log_exception_trace_outter"

#define LOG_EXCEPTION(LOGGER_FUNC, MESSAGE, INNER, OUTTER) 		\
	try {														\
		try {													\
			throw std::runtime_error(INNER);					\
		} catch (const std::exception& e) {						\
			std::throw_with_nested(std::runtime_error(OUTTER));	\
		}														\
	} catch (const std::exception& e) {							\
		LOGGER.LOGGER_FUNC(MESSAGE, e);							\
	}


static void logEntriesOnAllLevels() {
	LOGGER.error(LOG_ENTRY_ERROR);
	LOGGER.warning(LOG_ENTRY_WARNING);
	LOGGER.info(LOG_ENTRY_INFO);
	LOGGER.debug(LOG_ENTRY_DEBUG);
	LOGGER.trace(LOG_ENTRY_TRACE);
}

static void logExceptionsOnAllLevels() {
	LOG_EXCEPTION(error, LOG_EXCEPTION_ERROR_MESSAGE, LOG_EXCEPTION_ERROR_INNER, LOG_EXCEPTION_ERROR_OUTTER);
	LOG_EXCEPTION(warning, LOG_EXCEPTION_WARNING_MESSAGE, LOG_EXCEPTION_WARNING_INNER, LOG_EXCEPTION_WARNING_OUTTER);
	LOG_EXCEPTION(info, LOG_EXCEPTION_INFO_MESSAGE, LOG_EXCEPTION_INFO_INNER, LOG_EXCEPTION_INFO_OUTTER);
	LOG_EXCEPTION(debug, LOG_EXCEPTION_DEBUG_MESSAGE, LOG_EXCEPTION_DEBUG_INNER, LOG_EXCEPTION_DEBUG_OUTTER);
	LOG_EXCEPTION(trace, LOG_EXCEPTION_TRACE_MESSAGE, LOG_EXCEPTION_TRACE_INNER, LOG_EXCEPTION_TRACE_OUTTER);
}


TEST(LoggerTest, logTextOff) {
	std::shared_ptr<std::ostringstream> o = std::make_shared<std::ostringstream>();

	LOGGER.setOutputStream(o);
	LOGGER.setLevel(LogLevel::OFF);

	logEntriesOnAllLevels();

	EXPECT_EQ(std::string::npos, o->str().find(LOG_ENTRY_ERROR));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_ENTRY_WARNING));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_ENTRY_INFO));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_ENTRY_DEBUG));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_ENTRY_TRACE));
}

TEST(LoggerTest, logExceptionOff) {
	std::shared_ptr<std::ostringstream> o = std::make_shared<std::ostringstream>();

	LOGGER.setOutputStream(o);
	LOGGER.setLevel(LogLevel::OFF);

	logExceptionsOnAllLevels();

	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_ERROR_MESSAGE));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_ERROR_INNER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_ERROR_OUTTER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_WARNING_MESSAGE));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_WARNING_INNER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_WARNING_OUTTER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_INFO_MESSAGE));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_INFO_INNER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_INFO_OUTTER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_DEBUG_MESSAGE));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_DEBUG_INNER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_DEBUG_OUTTER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_TRACE_MESSAGE));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_TRACE_INNER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_TRACE_OUTTER));
}

TEST(LoggerTest, logTextError) {
	std::shared_ptr<std::ostringstream> o = std::make_shared<std::ostringstream>();

	LOGGER.setOutputStream(o);
	LOGGER.setLevel(LogLevel::ERROR);

	logEntriesOnAllLevels();

	EXPECT_NE(std::string::npos, o->str().find(LOG_ENTRY_ERROR));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_ENTRY_WARNING));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_ENTRY_INFO));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_ENTRY_DEBUG));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_ENTRY_TRACE));
}

TEST(LoggerTest, logExceptionError) {
	std::shared_ptr<std::ostringstream> o = std::make_shared<std::ostringstream>();

	LOGGER.setOutputStream(o);
	LOGGER.setLevel(LogLevel::ERROR);

	logExceptionsOnAllLevels();

	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_ERROR_MESSAGE));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_ERROR_INNER));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_ERROR_OUTTER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_WARNING_MESSAGE));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_WARNING_INNER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_WARNING_OUTTER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_INFO_MESSAGE));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_INFO_INNER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_INFO_OUTTER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_DEBUG_MESSAGE));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_DEBUG_INNER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_DEBUG_OUTTER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_TRACE_MESSAGE));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_TRACE_INNER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_TRACE_OUTTER));
}

TEST(LoggerTest, logTextWarning) {
	std::shared_ptr<std::ostringstream> o = std::make_shared<std::ostringstream>();

	LOGGER.setOutputStream(o);
	LOGGER.setLevel(LogLevel::WARNING);

	logEntriesOnAllLevels();

	EXPECT_NE(std::string::npos, o->str().find(LOG_ENTRY_ERROR));
	EXPECT_NE(std::string::npos, o->str().find(LOG_ENTRY_WARNING));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_ENTRY_INFO));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_ENTRY_DEBUG));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_ENTRY_TRACE));
}

TEST(LoggerTest, logExceptionWarning) {
	std::shared_ptr<std::ostringstream> o = std::make_shared<std::ostringstream>();

	LOGGER.setOutputStream(o);
	LOGGER.setLevel(LogLevel::WARNING);

	logExceptionsOnAllLevels();

	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_ERROR_MESSAGE));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_ERROR_INNER));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_ERROR_OUTTER));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_WARNING_MESSAGE));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_WARNING_INNER));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_WARNING_OUTTER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_INFO_MESSAGE));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_INFO_INNER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_INFO_OUTTER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_DEBUG_MESSAGE));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_DEBUG_INNER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_DEBUG_OUTTER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_TRACE_MESSAGE));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_TRACE_INNER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_TRACE_OUTTER));
}

TEST(LoggerTest, logTextInfo) {
	std::shared_ptr<std::ostringstream> o = std::make_shared<std::ostringstream>();

	LOGGER.setOutputStream(o);
	LOGGER.setLevel(LogLevel::INFO);

	logEntriesOnAllLevels();

	EXPECT_NE(std::string::npos, o->str().find(LOG_ENTRY_ERROR));
	EXPECT_NE(std::string::npos, o->str().find(LOG_ENTRY_WARNING));
	EXPECT_NE(std::string::npos, o->str().find(LOG_ENTRY_INFO));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_ENTRY_DEBUG));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_ENTRY_TRACE));
}

TEST(LoggerTest, logExceptionInfo) {
	std::shared_ptr<std::ostringstream> o = std::make_shared<std::ostringstream>();

	LOGGER.setOutputStream(o);
	LOGGER.setLevel(LogLevel::INFO);

	logExceptionsOnAllLevels();

	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_ERROR_MESSAGE));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_ERROR_INNER));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_ERROR_OUTTER));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_WARNING_MESSAGE));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_WARNING_INNER));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_WARNING_OUTTER));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_INFO_MESSAGE));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_INFO_INNER));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_INFO_OUTTER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_DEBUG_MESSAGE));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_DEBUG_INNER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_DEBUG_OUTTER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_TRACE_MESSAGE));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_TRACE_INNER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_TRACE_OUTTER));
}

TEST(LoggerTest, logTextDebug) {
	std::shared_ptr<std::ostringstream> o = std::make_shared<std::ostringstream>();

	LOGGER.setOutputStream(o);
	LOGGER.setLevel(LogLevel::DEBUG);

	logEntriesOnAllLevels();

	EXPECT_NE(std::string::npos, o->str().find(LOG_ENTRY_ERROR));
	EXPECT_NE(std::string::npos, o->str().find(LOG_ENTRY_WARNING));
	EXPECT_NE(std::string::npos, o->str().find(LOG_ENTRY_INFO));
	EXPECT_NE(std::string::npos, o->str().find(LOG_ENTRY_DEBUG));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_ENTRY_TRACE));
}

TEST(LoggerTest, logExceptionDebug) {
	std::shared_ptr<std::ostringstream> o = std::make_shared<std::ostringstream>();

	LOGGER.setOutputStream(o);
	LOGGER.setLevel(LogLevel::DEBUG);

	logExceptionsOnAllLevels();

	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_ERROR_MESSAGE));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_ERROR_INNER));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_ERROR_OUTTER));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_WARNING_MESSAGE));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_WARNING_INNER));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_WARNING_OUTTER));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_INFO_MESSAGE));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_INFO_INNER));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_INFO_OUTTER));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_DEBUG_MESSAGE));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_DEBUG_INNER));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_DEBUG_OUTTER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_TRACE_MESSAGE));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_TRACE_INNER));
	EXPECT_EQ(std::string::npos, o->str().find(LOG_EXCEPTION_TRACE_OUTTER));
}

TEST(LoggerTest, logTextTrace) {
	std::shared_ptr<std::ostringstream> o = std::make_shared<std::ostringstream>();

	LOGGER.setOutputStream(o);
	LOGGER.setLevel(LogLevel::TRACE);

	logEntriesOnAllLevels();

	EXPECT_NE(std::string::npos, o->str().find(LOG_ENTRY_ERROR));
	EXPECT_NE(std::string::npos, o->str().find(LOG_ENTRY_WARNING));
	EXPECT_NE(std::string::npos, o->str().find(LOG_ENTRY_INFO));
	EXPECT_NE(std::string::npos, o->str().find(LOG_ENTRY_DEBUG));
	EXPECT_NE(std::string::npos, o->str().find(LOG_ENTRY_TRACE));
}

TEST(LoggerTest, logExceptionTrace) {
	std::shared_ptr<std::ostringstream> o = std::make_shared<std::ostringstream>();

	LOGGER.setOutputStream(o);
	LOGGER.setLevel(LogLevel::TRACE);

	logExceptionsOnAllLevels();

	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_ERROR_MESSAGE));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_ERROR_INNER));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_ERROR_OUTTER));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_WARNING_MESSAGE));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_WARNING_INNER));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_WARNING_OUTTER));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_INFO_MESSAGE));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_INFO_INNER));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_INFO_OUTTER));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_DEBUG_MESSAGE));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_DEBUG_INNER));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_DEBUG_OUTTER));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_TRACE_MESSAGE));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_TRACE_INNER));
	EXPECT_NE(std::string::npos, o->str().find(LOG_EXCEPTION_TRACE_OUTTER));
}

TEST(LoggerTest, getLevelText) {
	EXPECT_EQ("OFF", to_string(LogLevel::OFF));
	EXPECT_EQ("ERROR", to_string(LogLevel::ERROR));
	EXPECT_EQ("WARNING", to_string(LogLevel::WARNING));
	EXPECT_EQ("INFO", to_string(LogLevel::INFO));
	EXPECT_EQ("DEBUG", to_string(LogLevel::DEBUG));
	EXPECT_EQ("TRACE", to_string(LogLevel::TRACE));
}

TEST(LoggerTest, isLoggableOFF) {
	LOGGER.setLevel(LogLevel::OFF);
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::ERROR));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::WARNING));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::INFO));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::DEBUG));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::TRACE));
}

TEST(LoggerTest, isLoggableError) {
	LOGGER.setLevel(LogLevel::ERROR);
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::ERROR));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::WARNING));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::INFO));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::DEBUG));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::TRACE));
}

TEST(LoggerTest, isLoggableWarning) {
	LOGGER.setLevel(LogLevel::WARNING);
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::ERROR));
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::WARNING));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::INFO));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::DEBUG));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::TRACE));
}

TEST(LoggerTest, isLoggableInfo) {
	LOGGER.setLevel(LogLevel::INFO);
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::ERROR));
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::WARNING));
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::INFO));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::DEBUG));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::TRACE));
}

TEST(LoggerTest, isLoggableDebug) {
	LOGGER.setLevel(LogLevel::DEBUG);
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::ERROR));
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::WARNING));
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::INFO));
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::DEBUG));
	EXPECT_FALSE(LOGGER.isLoggable(LogLevel::TRACE));
}

TEST(LoggerTest, isLoggableTrace) {
	LOGGER.setLevel(LogLevel::TRACE);
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::ERROR));
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::WARNING));
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::INFO));
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::DEBUG));
	EXPECT_TRUE(LOGGER.isLoggable(LogLevel::TRACE));
}

TEST(LoggerTest, setFileName) {
	std::string fileName = tmpnam(nullptr);
	EXPECT_NO_THROW(LOGGER.setFileName(fileName));

	std::ifstream is(fileName);
	EXPECT_TRUE(is.good());
}

TEST(LoggerTest, setFileNameInvalid) {
	EXPECT_THROW(LOGGER.setFileName("/////////"), IOException);
}

TEST(LoggerTest, toDto) {
	struct tm timeinfo;

	const time_t now1 = time(nullptr);

	std::ostringstream now1Str;
	now1Str << std::put_time(localtime_r(&now1, &timeinfo), "%Y.%m.%d %H:%M:%S");

	LOGGER.setLevel(LogLevel::INFO);

	LOGGER.error(LOG_ENTRY_ERROR);
	void (Logger::*warningFunc)(const char* format, ...) = &Logger::warning;
	void (Logger::*infoFunc)(const char* format, ...) = &Logger::info;

	LOGGER.error(LOG_ENTRY_ERROR);
	std::ostringstream oss0;
	oss0 << std::setw(8) << std::setfill('0') << std::hex << std::showbase << std::this_thread::get_id();

	std::thread t1(warningFunc, &LOGGER, LOG_ENTRY_WARNING);
	std::ostringstream oss1;
	oss1 << std::setw(8) << std::setfill('0') << std::hex << std::showbase << t1.get_id();
	t1.join();

	std::thread t2(infoFunc, &LOGGER, LOG_ENTRY_INFO);
	std::ostringstream oss2;
	oss2 << std::setw(8) << std::setfill('0') << std::hex << std::showbase << t2.get_id();
	t2.join();

	const time_t now2 = now1 + 1;
	std::ostringstream now2Str;
	now2Str << std::put_time(localtime_r(&now2, &timeinfo), "%Y.%m.%d %H:%M:%S");

	const auto logEntryDtoList = LOGGER.getEntries();

	EXPECT_THAT(logEntryDtoList, SizeIs(Ge(3)));

	EXPECT_THAT(std::next(logEntryDtoList.rbegin(), 0)->getTime(), AnyOf(now1Str.str(), now2Str.str()));
	EXPECT_THAT(std::next(logEntryDtoList.rbegin(), 0)->getThread(), Eq(oss2.str()));
	EXPECT_THAT(std::next(logEntryDtoList.rbegin(), 0)->getLevel(), Eq("INFO"));
	EXPECT_THAT(std::next(logEntryDtoList.rbegin(), 0)->getText(), Eq(LOG_ENTRY_INFO));

	EXPECT_THAT(std::next(logEntryDtoList.rbegin(), 1)->getTime(), AnyOf(now1Str.str(), now2Str.str()));
	EXPECT_THAT(std::next(logEntryDtoList.rbegin(), 1)->getThread(), Eq(oss1.str()));
	EXPECT_THAT(std::next(logEntryDtoList.rbegin(), 1)->getLevel(), Eq("WARNING"));
	EXPECT_THAT(std::next(logEntryDtoList.rbegin(), 1)->getText(), Eq(LOG_ENTRY_WARNING));

	EXPECT_THAT(std::next(logEntryDtoList.rbegin(), 2)->getTime(), AnyOf(now1Str.str(), now2Str.str()));
	EXPECT_THAT(std::next(logEntryDtoList.rbegin(), 2)->getThread(), Eq(oss0.str()));
	EXPECT_THAT(std::next(logEntryDtoList.rbegin(), 2)->getLevel(), Eq("ERROR"));
	EXPECT_THAT(std::next(logEntryDtoList.rbegin(), 2)->getText(), Eq(LOG_ENTRY_ERROR));
}
