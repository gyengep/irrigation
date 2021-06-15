<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="/">
		<html>
			<head>
				<title>Irrigation System Programs</title>
				<link rel="stylesheet" href="/resources/styles.css"/>
				<meta name="viewport" content="width=device-width, initial-scale=1"/>
			</head>
			<body>
				<table class="hoverable striped-table bordered-table">
					<caption>Irrigation System Programs</caption>
					<thead>
						<tr>
							<th>Enabled</th>
							<th>Name</th>
							<th>Adjustment</th>
							<th>Scheduler</th>
						</tr>
					</thead>
					<tbody>
						<xsl:for-each select="programs/program">
							<xsl:variable name="programId"><xsl:value-of select="@id" /></xsl:variable>
							<tr onclick="window.location='/programs/{$programId}'">
								<xsl:choose>
									<xsl:when test="enabled='true'">
										<td><input type="checkbox" checked="" onclick="return false"/></td>
									</xsl:when>
									<xsl:otherwise>
										<td><input type="checkbox" onclick="return false"/></td>
									</xsl:otherwise>
								</xsl:choose>
								<td><div class="contentcell nowordwrap"><xsl:value-of select="name"/></div></td>
								<td><div class="contentcell nowordwrap"><xsl:value-of select="format-number(adjustment*0.01, '#%')"/></div></td>
								<td><div class="contentcell nowordwrap"><xsl:value-of select="schedulertype"/></div></td>
							</tr>
						</xsl:for-each>
					</tbody>
				</table>
			</body>
		</html>
	</xsl:template>
</xsl:stylesheet>