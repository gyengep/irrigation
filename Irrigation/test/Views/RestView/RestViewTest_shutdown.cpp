#include "RestViewTest.h"
#include "Mocks/MockWateringController.h"
#include "Dto2Object/RunTimeListSamples.h"
#include "Model/IrrigationDocument.h"
#include "Views/RestView/RestView.h"
#include "Views/RestView/XmlIrrigationActionReader.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;


string RestViewTest::createPoweroffUrl() {
	return createUrl("/shutdown/poweroff");
}

string RestViewTest::createRebootUrl() {
	return createUrl("/shutdown/reboot");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, patchPowerOff) {
	EXPECT_CALL(*mockShutdownManager, powerOff()).Times(1);
	Response response = executeRequest("PATCH", createPoweroffUrl(), "", "application/xml");
	checkResponseWithoutBody(response, 204);
}

TEST_F(RestViewTest, patchReboot) {
	EXPECT_CALL(*mockShutdownManager, reboot()).Times(1);
	Response response = executeRequest("PATCH", createRebootUrl(), "", "application/xml");
	checkResponseWithoutBody(response, 204);
}
