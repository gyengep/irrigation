#pragma once
#include <gmock/gmock.h>
#include "Utils/FileWriterImpl.h"


class FileWriterImplTest : public ::testing::Test  {
public:
	static std::string getFileContent(const std::string& fileName);
};
