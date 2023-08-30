		
function markTopMenu(menuItem) {
  var i, tablinks;
 
 tablinks = document.getElementsByClassName("topMenuItem");
  for (i = 0; i < tablinks.length; i++) {
    tablinks[i].className = tablinks[i].className.replace(" w3-border-blue", "");
  }

  document.getElementById(menuItem).className += " w3-border-blue";
}

function markLeftMenu(menuItem) {
  var i, tablinks;

  tablinks = document.getElementsByClassName("leftMenuItem");
  for (i = 0; i < tablinks.length; i++) {
    tablinks[i].className = tablinks[i].className.replace(" w3-border-blue", "");
  }

  document.getElementById(menuItem).className += " w3-border-blue";
}

function markMenu(topMenuItem, leftMenuItem) {
  markTopMenu(topMenuItem);
  markLeftMenu(leftMenuItem);
}

function openLeftMenu() {
  document.getElementById("leftMenu").style.display = "block";
  document.getElementById("openMenuButton").style.display = "none";
  document.getElementById("closeMenuButton").style.display = "block";
}

function closeLeftMenu() {
  document.getElementById("leftMenu").style.display = "none";
  document.getElementById("openMenuButton").style.display = "block";
  document.getElementById("closeMenuButton").style.display = "none";
}

function accord(id) {
  var x = document.getElementById(id);

  if (x.className.indexOf("w3-show") == -1) {
    x.className += " w3-show";
  } else { 
    x.className = x.className.replace(" w3-show", "");
  }
}
