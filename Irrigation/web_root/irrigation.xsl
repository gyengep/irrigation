<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="/">
		<html>
			<head>
				<title>Irrigation System</title>
				<meta name="viewport" content="width=device-width, initial-scale=1"/>
				<link rel="stylesheet" href="/css/w3.css"/>
				<link rel="stylesheet" href="/css/my-styles.css"/>
				<script src="/scripts/html-duration-picker.min.js"></script>
				<script src="/scripts/my-scripts.js"></script>
				<script src="//ajax.googleapis.com/ajax/libs/jquery/1.9.1/jquery.min.js"></script>
				<script>
					function startIrrigation_custom() {
					  const runTimes = [
						[ document.getElementById('zone1').value.substring(0, 2), document.getElementById('zone1').value.substring(3, 5) ],
						[ document.getElementById('zone2').value.substring(0, 2), document.getElementById('zone2').value.substring(3, 5) ],
						[ document.getElementById('zone3').value.substring(0, 2), document.getElementById('zone3').value.substring(3, 5) ],
						[ document.getElementById('zone4').value.substring(0, 2), document.getElementById('zone4').value.substring(3, 5) ],
						[ document.getElementById('zone5').value.substring(0, 2), document.getElementById('zone5').value.substring(3, 5) ],
						[ document.getElementById('zone6').value.substring(0, 2), document.getElementById('zone6').value.substring(3, 5) ]
					  ];
					  
                      sendStartIrrigation_custom('100', runTimes);
					}
				</script>			
		</head>
			<body class="my-container">
			
				<table class="my-table w3-hoverable w3-border">
					<!--div class="my-table-caption">Run Times (mm:ss)</div-->

					<tr>
						<td class="w3-row">
							<div class="w3-col s4">
								<label class="my-cell-caption2" for="zone1">Zone 1</label>
							</div>
							<div class="w3-col s8">
								<div class="w3-right">
									<input type="text" class="html-duration-picker my-cell-content2" id="zone1" data-duration="00:00:00" data-hide-seconds="" />
								</div>
							</div>
						</td>
					</tr>

					<tr>
						<td class="w3-row">
							<div class="w3-col s4">
								<label class="my-cell-caption2" for="zone2">Zone 2</label>
							</div>
							<div class="w3-col s8">
								<div class="w3-right">
									<input type="text" class="html-duration-picker my-cell-content2" id="zone2" data-duration="00:00:00" data-hide-seconds="" />
								</div>
							</div>
						</td>
					</tr>

					<tr>
						<td class="w3-row">
							<div class="w3-col s4">
								<label class="my-cell-caption2" for="zone3">Zone 3</label>
							</div>
							<div class="w3-col s8">
								<div class="w3-right">
									<input type="text" class="html-duration-picker my-cell-content2" id="zone3" data-duration="00:00:00" data-hide-seconds="" />
								</div>
							</div>
						</td>
					</tr>

					<tr>
						<td class="w3-row">
							<div class="w3-col s4">
								<label class="my-cell-caption2" for="zone4">Zone 4</label>
							</div>
							<div class="w3-col s8">
								<div class="w3-right">
									<input type="text" class="html-duration-picker my-cell-content2" id="zone4" data-duration="00:00:00" data-hide-seconds="" />
								</div>
							</div>
						</td>
					</tr>

					<tr>
						<td class="w3-row">
							<div class="w3-col s4">
								<label class="my-cell-caption2" for="zone5">Zone 5</label>
							</div>
							<div class="w3-col s8">
								<div class="w3-right">
									<input type="text" class="html-duration-picker my-cell-content2" id="zone5" data-duration="00:00:00" data-hide-seconds="" />
								</div>
							</div>
						</td>
					</tr>

					<tr>
						<td class="w3-row">
							<div class="w3-col s4">
								<label class="my-cell-caption2" for="zone6">Zone 6</label>
							</div>
							<div class="w3-col s8">
								<div class="w3-right">
									<input type="text" class="html-duration-picker my-cell-content2" id="zone6" data-duration="00:00:00" data-hide-seconds="" />
								</div>
							</div>
						</td>
					</tr>
				</table>

				<div class="w3-row">
					<div class="w3-col" style="width:50%">
						<button class="w3-btn w3-light-gray w3-round w3-block w3-left" style="width:calc(100% - 8px)" onclick="startIrrigation_custom()">Start</button>
					</div>

					<div class="w3-col" style="width:50%">
						<button class="w3-btn w3-light-gray w3-round w3-block w3-right" style="width:calc(100% - 8px)" onclick="sendStopIrrigation()">Stop</button>
					</div>
				</div>

			</body>
		</html>
	</xsl:template>
</xsl:stylesheet>
