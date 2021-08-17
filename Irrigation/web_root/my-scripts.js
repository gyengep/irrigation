
function sendStart(programId, adjustment) {
  event.preventDefault();
  event.stopPropagation();

  var data = '<irrigation><action>start-program</action><program-id>' + programId + '</program-id><adjustment>' + adjustment + '</adjustment></irrigation>';
  $.ajax({
    type: "PATCH",
    url: "api/v1/irrigation",
    dataType: "xml",
    data: data,
    contentType: "application/xml",
	
    success: function (res) {
        alert("Irrigation started!");
    },

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
/*	
    success: function (res) {
        alert("Program is enabled!");
    },
*/
    error: function (res) {
        alert("XML: not working! " + res.statusText);
    }
  });
}
