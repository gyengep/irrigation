<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="/">
		<html>
			<head>
				<title>Irrigation System</title>
				<meta name="viewport" content="width=device-width, initial-scale=1"/>
				<link rel="stylesheet" href="/css/w3.css"/>
				<link rel="stylesheet" href="/css/my-styles.css"/>
			</head>
			<body class="my-container">
			
				<table class="my-table w3-responsive">
					<xsl:for-each select="log_entries/log_entry">
						<tr>
							<td>
								<div class="w3-row my-cell-description">
									<xsl:value-of select="time"/> - <xsl:value-of select="level"/> - <xsl:value-of select="thread"/>
								</div>
								<div class="w3-row my-cell-caption">
									<div class="contentcell"><xsl:value-of select="text"/></div>
								</div>
							</td>
						</tr>
					</xsl:for-each>
				</table>
				
			</body>
		</html>
	</xsl:template>
</xsl:stylesheet>
