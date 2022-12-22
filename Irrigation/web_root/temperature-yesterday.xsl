<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="/">
		<html>
			<head>
				<title>Irrigation System</title>
				<meta name="viewport" content="width=device-width, initial-scale=1"/>
				<link rel="stylesheet" href="/w3.css"/>
				<link rel="stylesheet" href="/my-styles.css"/>
			</head>
			<body class="my-container">
			
				<div class="w3-bar">
					<div class="w3-bar-item w3-left">Yesterday's temperature</div>
					<div class="w3-bar-item w3-right"><xsl:value-of select="temperature/historical/datetime-from"/></div>
				</div>
				
				<div class="w3-center w3-jumbo"><xsl:value-of select="temperature/historical/value-min"/> / <xsl:value-of select="temperature/historical/value-max"/><xsl:value-of select="temperature/unit"/></div>
					
			</body>
		</html>
	</xsl:template>
</xsl:stylesheet>
