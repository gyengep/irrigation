#include "common.h"
#include "Commons/Logger.h"


TEST(LoggerTest, isLoggableError) {
	LOGGER.setLevel(Logger::ERROR);
	EXPECT_TRUE(LOGGER.isLoggable(Logger::ERROR));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::WARNING));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::INFO));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::DEBUG));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::TRACE));
}

TEST(LoggerTest, isLoggableWarning) {
	LOGGER.setLevel(Logger::WARNING);
	EXPECT_TRUE(LOGGER.isLoggable(Logger::ERROR));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::WARNING));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::INFO));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::DEBUG));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::TRACE));
}

TEST(LoggerTest, isLoggableInfo) {
	LOGGER.setLevel(Logger::INFO);
	EXPECT_TRUE(LOGGER.isLoggable(Logger::ERROR));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::WARNING));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::INFO));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::DEBUG));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::TRACE));
}

TEST(LoggerTest, isLoggableDebug) {
	LOGGER.setLevel(Logger::DEBUG);
	EXPECT_TRUE(LOGGER.isLoggable(Logger::ERROR));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::WARNING));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::INFO));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::DEBUG));
	EXPECT_FALSE(LOGGER.isLoggable(Logger::TRACE));
}

TEST(LoggerTest, isLoggableTrace) {
	LOGGER.setLevel(Logger::TRACE);
	EXPECT_TRUE(LOGGER.isLoggable(Logger::ERROR));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::WARNING));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::INFO));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::DEBUG));
	EXPECT_TRUE(LOGGER.isLoggable(Logger::TRACE));
}
