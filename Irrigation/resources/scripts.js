
function sendStart(programId, adjustment) {
  event.preventDefault();
  event.stopPropagation();

  var data = '<irrigation><action>start-program</action><program-id>' + programId + '</program-id><adjustment>' + adjustment + '</adjustment></irrigation>';
  $.ajax({
    type: "PATCH",
    url: "/irrigation",
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
