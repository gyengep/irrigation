<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="/">
		<html>
			<head>
				<title>Irrigation System Programs</title>
				<meta name="viewport" content="width=device-width, initial-scale=1"/>
				<link rel="stylesheet" href="/resources/w3.css"/>
				<link rel="stylesheet" href="/resources/styles.css"/>
				<script src="//ajax.googleapis.com/ajax/libs/jquery/1.9.1/jquery.min.js"></script>
				<script src="/resources/scripts.js"></script>
			</head>
			<body>
				<div class="w3-container w3-sans-serif">
					<table class="w3-table-all w3-hoverable w3-card-4 table-100percent">
						<thead>
							<tr>
								<th>Enabled</th>
								<th>Name</th>
								<th>Adjustment</th>
								<th>Scheduler</th>
								<th></th>
							</tr>
						</thead>
						<tbody>
							<xsl:for-each select="programs/program">
								<xsl:variable name="programId"><xsl:value-of select="@id"/></xsl:variable>
								<xsl:variable name="adjustment"><xsl:value-of select="adjustment"/></xsl:variable>
								<tr onclick="window.location='/programs/{$programId}'">
									<xsl:choose>
										<xsl:when test="enabled='true'">
											<td><input type="checkbox" onclick="return false" checked="checked"/></td>
										</xsl:when>
										<xsl:otherwise>
											<td><input type="checkbox" onclick="return false"/></td>
										</xsl:otherwise>
									</xsl:choose>
									<td><div class="contentcell nowordwrap"><xsl:value-of select="name"/></div></td>
									<td><div class="contentcell nowordwrap"><xsl:value-of select="format-number(adjustment*0.01, '#%')"/></div></td>
									<td><div class="contentcell nowordwrap"><xsl:value-of select="schedulertype"/></div></td>
									<td><button class="w3-round" onclick="sendStart({$programId}, {$adjustment})">Start</button></td>
								</tr>
							</xsl:for-each>
						</tbody>
					</table>
				</div>
			</body>
		</html>
	</xsl:template>
</xsl:stylesheet>
