#include "RestViewTest.h"
#include "Mocks/MockWateringController.h"
#include "Dto2Object/RunTimeListSamples.h"
#include "Model/IrrigationDocument.h"
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
	const unsigned expectedAdjustment = 50;
	const shared_ptr<MockWateringController> mockWateringController(new MockWateringController());
	const std::list<std::chrono::seconds> expectedDurations {
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

	EXPECT_CALL(*mockWateringController, start(expectedDurations, expectedAdjustment));

	irrigationDocument = IrrigationDocument::Builder().setWateringController(mockWateringController).build();
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

TEST_F(RestViewTest, patchIrrigationActionStart2) {
	const unsigned expectedAdjustment = 70;
	const shared_ptr<MockWateringController> mockWateringController(new MockWateringController());
	const std::list<std::chrono::seconds> expectedDurations {
		std::chrono::minutes(0) + std::chrono::seconds(9),
		std::chrono::minutes(1) + std::chrono::seconds(8),
		std::chrono::minutes(2) + std::chrono::seconds(7),
		std::chrono::minutes(3) + std::chrono::seconds(6),
		std::chrono::minutes(4) + std::chrono::seconds(5),
		std::chrono::minutes(5) + std::chrono::seconds(4),
	};

	const string xml =
			"<irrigation>"
				"<action>start</action>"
				"<adjustment>" + to_string(expectedAdjustment) + "</adjustment>"
				"<runtimes>"
					"<runtime><minute>0</minute><second>9</second></runtime>"
					"<runtime><minute>1</minute><second>8</second></runtime>"
					"<runtime><minute>2</minute><second>7</second></runtime>"
					"<runtime><minute>3</minute><second>6</second></runtime>"
					"<runtime><minute>4</minute><second>5</second></runtime>"
					"<runtime><minute>5</minute><second>4</second></runtime>"
				"</runtimes>"
			"</irrigation>";

	EXPECT_CALL(*mockWateringController, start(expectedDurations, expectedAdjustment));

	irrigationDocument = IrrigationDocument::Builder().setWateringController(mockWateringController).build();
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
	const std::list<std::chrono::seconds> expectedDurations {
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

	EXPECT_CALL(*mockWateringController, start(expectedDurations, 100));

	irrigationDocument = IrrigationDocument::Builder().setWateringController(mockWateringController).build();
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

	const string xml =
			"<irrigation>"
				"<action>start-program</action>"
				"<adjustment>" + to_string(expectedAdjustment) + "</adjustment>"
				"<program-id>" + programListSample.getContainerPtr()->begin()->first.toString() + "</program-id>"
			"</irrigation>";

	EXPECT_CALL(
		*mockWateringController,
		start(
			programListSample.getContainerPtr()->begin()->second->getRunTimeContainer().toDurations(),
			expectedAdjustment
		)
	);

	irrigationDocument = IrrigationDocument::Builder().
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

	const string xml =
			"<irrigation>"
				"<action>start-program</action>"
				"<program-id>" + programListSample.getContainerPtr()->begin()->first.toString() + "</program-id>"
			"</irrigation>";

	EXPECT_CALL(
		*mockWateringController,
		start(
			programListSample.getContainerPtr()->begin()->second->getRunTimeContainer().toDurations(),
			programListSample.getContainerPtr()->begin()->second->getAdjustment()
		)
	);

	irrigationDocument = IrrigationDocument::Builder().
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

	irrigationDocument = IrrigationDocument::Builder().setWateringController(mockWateringController).build();
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

