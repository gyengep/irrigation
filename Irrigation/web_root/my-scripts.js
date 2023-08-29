
function sendStartIrrigation_program(adjustment, programId) {
  event.preventDefault();
  event.stopPropagation();

  var data = '<irrigation><action>start-program</action><program-id>' + programId + '</program-id><adjustment>' + adjustment + '</adjustment></irrigation>';
  $.ajax({
    type: "PATCH",
    url: "/api/v1/irrigation",
    dataType: "xml",
    data: data,
    contentType: "application/xml",
	
//    success: function (res) {
//        alert("Irrigation started!");
//    },

    error: function (res) {
        alert("XML: not working! " + res.statusText);
    }
  });
}


function sendStartIrrigation_custom(adjustment, runTimes) {
  event.preventDefault();
  event.stopPropagation();

  var data = 
  '<irrigation>' +
  '<action>start</action>' + 
  '<adjustment>' + adjustment + '</adjustment>' +
  '<runtimes>';
  
  for (let i = 0; i < runTimes.length; i++) {
  	//data += '<runtime id="' + i + '"><minute>' + runTimes[i].substring(0, 2) + '</minute><second>' + runTimes[i].substring(3, 5) + '</second></runtime>';
  	data += '<runtime id="' + i + '"><minute>' + runTimes[i][0] + '</minute><second>' + runTimes[i][1] + '</second></runtime>';
  }
	
  data += 
  '</runtimes>' + 
  '</irrigation>';
  
  $.ajax({
    type: "PATCH",
    url: "/api/v1/irrigation",
    dataType: "xml",
    data: data,
    contentType: "application/xml",
	
//    success: function (res) {
//        alert("Irrigation started!");
//    },

    error: function (res) {
        alert("XML: not working! " + res.statusText);
    }
  });
}


function sendStopIrrigation() {
  event.preventDefault();
  event.stopPropagation();

  var data = '<irrigation><action>stop</action></irrigation>';
  $.ajax({
    type: "PATCH",
    url: "/api/v1/irrigation",
    dataType: "xml",
    data: data,
    contentType: "application/xml",
	
//    success: function (res) {
//        alert("Irrigation started!");
//    },

    error: function (res) {
        alert("XML: not working! " + res.statusText);
    }
  });
}


function sendEnableOrDisableProgram(programId, enabled) {
	
  var data = '<program><enabled>' + enabled + '</enabled></program>';
  $.ajax({
    type: "PATCH",
    url: "/api/v1/programs/" + programId,
    dataType: "xml",
    data: data,
    contentType: "application/xml",
	
//    success: function (res) {
//        alert("Program is enabled!");
//    },

    error: function (res) {
        alert("XML: not working! " + res.statusText);
    }
  });
}
