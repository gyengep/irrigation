<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="/">
		<html>
			<head>
				<title>Irrigation System Log</title>
				<meta name="viewport" content="width=device-width, initial-scale=1"/>
				<link rel="stylesheet" href="/resources/w3.css"/>
				<link rel="stylesheet" href="/resources/styles.css"/>
			</head>
			<body>
				<div class="w3-container w3-sans-serif">
					<table class="w3-table-all w3-card-4 table-100percent">
						<thead>
							<tr>
								<th>Date</th>
								<th>Level</th>
								<th>Thread</th>
								<th>Message</th>
							</tr>
						</thead>
						<tbody>
							<xsl:for-each select="log_entries/log_entry">
								<tr>
									<td><div class="contentcell nowordwrap"><xsl:value-of select="time"/></div></td>
									<td><div class="contentcell nowordwrap"><xsl:value-of select="level"/></div></td>
									<td><div class="contentcell nowordwrap"><xsl:value-of select="thread"/></div></td>
									<td><div class="contentcell"><xsl:value-of select="text"/></div></td>
								</tr>
							</xsl:for-each>
						</tbody>
					</table>
				</div>
			</body>
		</html>
	</xsl:template>
</xsl:stylesheet>
