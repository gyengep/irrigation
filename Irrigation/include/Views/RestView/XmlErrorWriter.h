#pragma once
#include "ErrorWriter.h"


class XmlErrorWriter : public ErrorWriter {
public:
	virtual ~XmlErrorWriter() = default;
	virtual std::string contentType() const override;
	virtual std::string toString(unsigned errorCode, const std::string& genericMessage, const std::string& detailedMessage) const override;
};
