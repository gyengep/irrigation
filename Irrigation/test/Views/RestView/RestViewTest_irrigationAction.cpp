#include "RestViewTest.h"
#include "Mocks/MockWateringController.h"
#include "TestCommon/cout.h"
#include "Dto2Object/RunTimeListSamples.h"
#include "Model/IrrigationDocumentImpl.h"
#include "Views/RestView/RestView.h"
#include "Views/RestView/XmlIrrigationActionReader.h"

using namespace std;
using namespace testing;
using namespace Dto2ObjectTest;


string RestViewTest::createIrrigationActionUrl() {
	return createUrl("/irrigation");
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, patchIrrigationActionStart1) {
	const unsigned expectedAdjustment = 75;
	const shared_ptr<MockWateringController> mockWateringController(new MockWateringController());
	const DurationList expectedDurations {
		std::chrono::seconds(15),
		std::chrono::seconds(30),
		std::chrono::seconds(45),
		std::chrono::seconds(60),
		std::chrono::seconds(75),
		std::chrono::seconds(90)
	};

	const string xml =
			"<irrigation>"
				"<action>start</action>"
				"<adjustment>" + to_string(expectedAdjustment) + "</adjustment>"
				"<runtimes>"
					"<runtime><minute>0</minute><second>15</second></runtime>"
					"<runtime><minute>0</minute><second>30</second></runtime>"
					"<runtime><minute>0</minute><second>45</second></runtime>"
					"<runtime><minute>1</minute><second>0</second></runtime>"
					"<runtime><minute>1</minute><second>15</second></runtime>"
					"<runtime><minute>1</minute><second>30</second></runtime>"
				"</runtimes>"
			"</irrigation>";

	EXPECT_CALL(*mockWateringController, start(expectedDurations.adjust(expectedAdjustment)));

	irrigationDocument = IrrigationDocumentImpl::Builder().setWateringController(mockWateringController).build();
	irrigationDocument->addView(unique_ptr<View>(new RestView(*irrigationDocument, port,
			mockCurrentTemperature,
			mockTemperatureForecast,
			mockTemperatureHistory,
			mockShutdownManager,
			"/tmp"
		)));

	Response response = executeRequest("PATCH", createIrrigationActionUrl(), xml, "application/xml");
	checkResponseWithoutBody(response, 200);
}

TEST_F(RestViewTest, patchIrrigationActionStartWithoutAdjustment) {
	const shared_ptr<MockWateringController> mockWateringController(new MockWateringController());
	const DurationList expectedDurations {
		std::chrono::seconds(15),
		std::chrono::seconds(30),
		std::chrono::seconds(45),
		std::chrono::seconds(60),
		std::chrono::seconds(75),
		std::chrono::seconds(90)
	};

	const string xml =
			"<irrigation>"
				"<action>start</action>"
				"<runtimes>"
					"<runtime><minute>0</minute><second>15</second></runtime>"
					"<runtime><minute>0</minute><second>30</second></runtime>"
					"<runtime><minute>0</minute><second>45</second></runtime>"
					"<runtime><minute>1</minute><second>0</second></runtime>"
					"<runtime><minute>1</minute><second>15</second></runtime>"
					"<runtime><minute>1</minute><second>30</second></runtime>"
				"</runtimes>"
			"</irrigation>";

	EXPECT_CALL(*mockWateringController, start(expectedDurations));

	irrigationDocument = IrrigationDocumentImpl::Builder().setWateringController(mockWateringController).build();
	irrigationDocument->addView(unique_ptr<View>(new RestView(*irrigationDocument, port,
			mockCurrentTemperature,
			mockTemperatureForecast,
			mockTemperatureHistory,
			mockShutdownManager,
			"/tmp"
		)));

	Response response = executeRequest("PATCH", createIrrigationActionUrl(), xml, "application/xml");
	checkResponseWithoutBody(response, 200);
}

TEST_F(RestViewTest, patchIrrigationActionStartProgramWithAdjustment) {
	const unsigned expectedAdjustment = 50;
	const ProgramListSample2 programListSample;
	const shared_ptr<MockWateringController> mockWateringController(new MockWateringController());

	const IdType id = programListSample.getContainerPtr()->begin()->first;
	const ProgramPtr program = programListSample.getContainerPtr()->begin()->second;

	const string xml =
			"<irrigation>"
				"<action>start-program</action>"
				"<adjustment>" + to_string(expectedAdjustment) + "</adjustment>"
				"<program-id>" + id.toString() + "</program-id>"
			"</irrigation>";

	EXPECT_CALL(
		*mockWateringController,
		start(
			program->getRunTimeContainer().toDurationList().adjust(program->getAdjustment()).adjust(expectedAdjustment)
		)
	);

	irrigationDocument = IrrigationDocumentImpl::Builder().
			setProgramContainer(programListSample.getContainerPtr()).
			setWateringController(mockWateringController).
			build();

	irrigationDocument->addView(unique_ptr<View>(new RestView(*irrigationDocument, port,
			mockCurrentTemperature,
			mockTemperatureForecast,
			mockTemperatureHistory,
			mockShutdownManager,
			"/tmp"
		)));

	Response response = executeRequest("PATCH", createIrrigationActionUrl(), xml, "application/xml");
	checkResponseWithoutBody(response, 200);
}

TEST_F(RestViewTest, patchIrrigationActionStartProgramWithoutAdjustment) {
	const ProgramListSample2 programListSample;
	const shared_ptr<MockWateringController> mockWateringController(new MockWateringController());

	const IdType id = programListSample.getContainerPtr()->begin()->first;
	const ProgramPtr program = programListSample.getContainerPtr()->begin()->second;

	const string xml =
			"<irrigation>"
				"<action>start-program</action>"
				"<program-id>" + id.toString() + "</program-id>"
			"</irrigation>";

	EXPECT_CALL(
		*mockWateringController,
		start(
				program->getRunTimeContainer().toDurationList().adjust(program->getAdjustment())
		)
	);

	irrigationDocument = IrrigationDocumentImpl::Builder().
			setProgramContainer(programListSample.getContainerPtr()).
			setWateringController(mockWateringController).
			build();

	irrigationDocument->addView(unique_ptr<View>(new RestView(*irrigationDocument, port,
			mockCurrentTemperature,
			mockTemperatureForecast,
			mockTemperatureHistory,
			mockShutdownManager,
			"/tmp"
		)));

	Response response = executeRequest("PATCH", createIrrigationActionUrl(), xml, "application/xml");
	checkResponseWithoutBody(response, 200);
}

TEST_F(RestViewTest, patchIrrigationActionStop) {
	const shared_ptr<MockWateringController> mockWateringController(new MockWateringController());

	const string xml =
			"<irrigation>"
				"<action>stop</action>"
			"</irrigation>";

	EXPECT_CALL(*mockWateringController, stop());

	irrigationDocument = IrrigationDocumentImpl::Builder().setWateringController(mockWateringController).build();
	irrigationDocument->addView(unique_ptr<View>(new RestView(*irrigationDocument, port,
			mockCurrentTemperature,
			mockTemperatureForecast,
			mockTemperatureHistory,
			mockShutdownManager,
			"/tmp"
		)));

	Response response = executeRequest("PATCH", createIrrigationActionUrl(), xml, "application/xml");
	checkResponseWithoutBody(response, 200);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(RestViewTest, patchIrrigationActionWithoutAction) {

	const string xml =
			"<irrigation>"
				"<adjustment>100</adjustment>"
				"<runtimes>"
					"<runtime><minute>0</minute><second>9</second></runtime>"
					"<runtime><minute>1</minute><second>8</second></runtime>"
					"<runtime><minute>2</minute><second>7</second></runtime>"
					"<runtime><minute>3</minute><second>6</second></runtime>"
					"<runtime><minute>4</minute><second>5</second></runtime>"
					"<runtime><minute>5</minute><second>4</second></runtime>"
				"</runtimes>"
			"</irrigation>";

	Response response = executeRequest("PATCH", createIrrigationActionUrl(), xml, "application/xml");
	checkErrorResponse(response, 400, "application/xml");
}

TEST_F(RestViewTest, patchIrrigationActionWithInvalidAction) {

	const string xml =
			"<irrigation>"
				"<action>abcd</action>"
				"<runtimes>"
					"<runtime><minute>0</minute><second>15</second></runtime>"
					"<runtime><minute>0</minute><second>30</second></runtime>"
					"<runtime><minute>0</minute><second>45</second></runtime>"
					"<runtime><minute>1</minute><second>0</second></runtime>"
					"<runtime><minute>1</minute><second>15</second></runtime>"
					"<runtime><minute>1</minute><second>30</second></runtime>"
				"</runtimes>"
			"</irrigation>";

	Response response = executeRequest("PATCH", createIrrigationActionUrl(), xml, "application/xml");
	checkErrorResponse(response, 400, "application/xml");
}

TEST_F(RestViewTest, patchIrrigationActionStartWithoutRunTime) {

	const string xml =
			"<irrigation>"
				"<action>start</action>"
			"</irrigation>";

	Response response = executeRequest("PATCH", createIrrigationActionUrl(), xml, "application/xml");
	checkErrorResponse(response, 400, "application/xml");
}

TEST_F(RestViewTest, patchIrrigationActionStartWithoutProgramId) {

	const string xml =
			"<irrigation>"
				"<action>start-program</action>"
			"</irrigation>";

	Response response = executeRequest("PATCH", createIrrigationActionUrl(), xml, "application/xml");
	checkErrorResponse(response, 400, "application/xml");
}

