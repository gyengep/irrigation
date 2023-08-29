<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="/">
		<html>
			<head>
				<title>Irrigation System</title>
				<meta name="viewport" content="width=device-width, initial-scale=1"/>
				<link rel="stylesheet" href="/css/w3.css"/>
				<link rel="stylesheet" href="/css/my-styles.css"/>
				<script src="/scripts/my-scripts.js"></script>
				<script src="//ajax.googleapis.com/ajax/libs/jquery/1.9.1/jquery.min.js"></script>
				<script>
					function startIrrigation_program() {
					  const queryString = window.location.search;
					  const urlParams = new URLSearchParams(queryString);
					  
					  console.log(queryString);
					  console.log(urlParams);
					  
                      sendStartIrrigation_program('100', urlParams.get('programId'));
					}
				</script>			
			</head>
			<body class="my-container">
					
				<table class="my-table w3-hoverable">
					<tr>
						<td class="w3-row">
							<div class="w3-col s4 my-cell-caption2">Name</div>
							<div class="w3-rest my-cell-content2"><xsl:value-of select="program/name"/></div>
						</td>
					</tr>
					<tr>
						<td class="w3-row">
							<div class="w3-col s4 my-cell-caption2">Adjustment</div>
							<div class="w3-rest my-cell-content2"><xsl:value-of select="format-number(program/adjustment*0.01, '#%')"/></div>
						</td>
					</tr>
					<tr>
						<td class="w3-row">
							<div class="w3-col s4 my-cell-caption2">Scheduler type</div>
							<div class="w3-rest my-cell-content2"><xsl:value-of select="program/schedulertype"/></div>
						</td>
					</tr>
				</table>	

				<table class="my-table w3-hoverable">
					<div class="my-table-caption">Start Times (hh:mm)</div>
					<xsl:for-each select="program/starttimes/starttime">
						<tr>
							<td class="w3-row">
								<div class="w3-rest my-cell-content2"><xsl:value-of select="format-number(hour, '00')"/>:<xsl:value-of select="format-number(minute, '00')"/></div>
							</td>
						</tr>
					</xsl:for-each>
				</table>	

				<table class="my-table w3-hoverable w3-border">
					<div class="my-table-caption">Run Times (mm:ss)</div>
					<xsl:for-each select="program/runtimes/runtime">
						<xsl:variable name="i" select="position()"/>
						<tr>
							<td class="w3-row">
								<div class="w3-col s4 my-cell-caption2">Zone <xsl:value-of select="$i" /></div>
								<div class="w3-rest my-cell-content2"><xsl:value-of select="format-number(minute, '00')"/>:<xsl:value-of select="format-number(second, '00')"/></div>
							</td>
						</tr>
					</xsl:for-each>
				</table>

				<div class="w3-row">
					<div class="w3-col" style="width:50%">
						<button class="w3-btn w3-light-gray w3-round w3-block w3-left" style="width:calc(100% - 8px)" onclick="startIrrigation_program()">Start</button>
					</div>

					<div class="w3-col" style="width:50%">
						<button class="w3-btn w3-light-gray w3-round w3-block w3-right" style="width:calc(100% - 8px)" onclick="sendStopIrrigation()">Stop</button>
					</div>
				</div>

			</body>
		</html>
	</xsl:template>
</xsl:stylesheet>
