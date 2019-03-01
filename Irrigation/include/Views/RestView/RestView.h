#pragma once
#include <memory>
#include "DocumentView/View.h"

class IrrigationDocument;
class RestService;
class WebServer;


class RestView : public View {
	const uint16_t port;

	std::shared_ptr<RestService> restService;
	std::unique_ptr<WebServer> webServer;

public:
	RestView(IrrigationDocument& irrigationDocument, uint16_t port);
	virtual ~RestView();

	virtual void initialize() override;
	virtual void terminate() override;
};
