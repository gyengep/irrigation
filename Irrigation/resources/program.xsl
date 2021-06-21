<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="/">
		<html>
			<head>
				<title>Irrigation System Program</title>
				<meta name="viewport" content="width=device-width, initial-scale=1"/>
				<link rel="stylesheet" href="/resources/w3.css"/>
				<link rel="stylesheet" href="/resources/styles.css"/>
			</head>
			<body>
				<div class="w3-container w3-sans-serif">
					<table class="w3-table w3-border w3-bordered w3-card-4 table-100percent">
						<tbody>
							<tr>
								<td><div class="captioncell">Name</div></td>
								<td><div class="contentcell"><xsl:value-of select="program/name"/></div></td>
							</tr>
							<tr>
								<td><div class="captioncell">Adjustment</div></td>
								<td><div class="contentcell"><xsl:value-of select="format-number(program/adjustment*0.01, '#%')"/></div></td>
							</tr>
							<tr>
								<td><div class="captioncell">Scheduler type</div></td>
								<td><div class="contentcell"><xsl:value-of select="program/schedulertype"/></div></td>
							</tr>

							<tr>
								<td><div class="groupingcell">Start times</div></td>
								<td/>
							</tr>
							<xsl:for-each select="program/starttimes/starttime">
								<tr>
									<td><div class="captioncell"></div></td>
									<td><div class="contentcell"><xsl:value-of select="format-number(hour, '00')"/>:<xsl:value-of select="format-number(minute, '00')"/></div></td>
								</tr>
							</xsl:for-each>

							<tr>
								<td><div class="groupingcell">Run times</div></td>
								<td/>
							</tr>
							<xsl:for-each select="program/runtimes/runtime">
								<xsl:variable name="i" select="position()"/>
								<tr>
									<td><div class="captioncell">Zone <xsl:value-of select="$i" /></div></td>
									<td><div class="contentcell"><xsl:value-of select="format-number(minute, '00')"/>:<xsl:value-of select="format-number(second, '00')"/></div></td>
								</tr>
							</xsl:for-each>
							
						</tbody>
					</table>
				</div>
			</body>
		</html>
	</xsl:template>
</xsl:stylesheet>
