#pragma once
#include <gmock/gmock.h>
#include "DtoReaderWriter/XmlReader.h"
#include "DtoReaderWriter/XmlWriter.h"
#include "TestCommon/XmlModify.h"

std::string remove_xml_tag(const std::string& text);
