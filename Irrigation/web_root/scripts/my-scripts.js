
function send_StartIrrigation_program(programId, adjustment) {
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


function send_StartIrrigation_custom(runTimes, adjustment) {
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


function send_StopIrrigation() {
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


function send_EnableOrDisableProgram(programId, enabled) {
	
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


function send_Shutdown() {
	
  $.ajax({
    type: "PATCH",
    url: "/api/v1/power/shutdown",
	
//    success: function (res) {
//        alert("Program is enabled!");
//    },

    error: function (res) {
        alert("Not working! " + res.statusText);
    }
  });
}


function send_Reboot() {
	
  $.ajax({
    type: "PATCH",
    url: "/api/v1/power/reboot",
	
//    success: function (res) {
//        alert("Program is enabled!");
//    },

    error: function (res) {
        alert("Not working! " + res.statusText);
    }
  });
}
