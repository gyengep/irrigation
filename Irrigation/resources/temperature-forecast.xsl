<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="/">
		<html>
			<head>
				<title>Irrigation System</title>
				<meta name="viewport" content="width=device-width, initial-scale=1"/>
				<link rel="stylesheet" href="/resources/w3.css"/>
				<link rel="stylesheet" href="/resources/styles.css"/>
			</head>
			<body>
				<div class="w3-container w3-sans-serif">
					<div class="w3-content" style="max-width: 600px">
						<div class="w3-bar" style="max-width: 600px">
							<div class="w3-bar-item w3-left">Temperature forecast</div>
							<div class="w3-bar-item w3-right"><xsl:value-of select="temperature-forecast/datetime-from"/></div>
						</div>
						<div class="w3-center w3-jumbo"><xsl:value-of select="temperature-forecast/value-min"/> / <xsl:value-of select="temperature-forecast/value-max"/>&#8451;</div>
					</div>
				</div>
			</body>
		</html>
	</xsl:template>
</xsl:stylesheet>
