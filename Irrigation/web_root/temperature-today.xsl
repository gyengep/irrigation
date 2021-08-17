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
					<div class="w3-bar-item w3-left">Today's temperature</div>
					<div class="w3-bar-item w3-right"><xsl:value-of select="temperature/historical/datetime-from"/></div>
				</div>

				<div class="w3-center w3-jumbo">
					<xsl:variable name="forecast-min"><xsl:value-of select="temperature/forecast/value-min"/></xsl:variable>
					<xsl:variable name="forecast-max"><xsl:value-of select="temperature/forecast/value-max"/></xsl:variable>
					<xsl:variable name="historical-min"><xsl:value-of select="temperature/historical/value-min"/></xsl:variable>
					<xsl:variable name="historical-max"><xsl:value-of select="temperature/historical/value-max"/></xsl:variable>

					<xsl:variable name="min-value">
						<xsl:choose>
							<xsl:when test="$forecast-min &lt; $historical-min">
								<xsl:copy-of select="$forecast-min" />
							</xsl:when>
							<xsl:otherwise>
								<xsl:copy-of select="$historical-min" />
							</xsl:otherwise>
						</xsl:choose>
					</xsl:variable>

					<xsl:variable name="max-value">
						<xsl:choose>
							<xsl:when test="$forecast-max &gt; $historical-max">
								<xsl:copy-of select="$forecast-max" />
							</xsl:when>
							<xsl:otherwise>
								<xsl:copy-of select="$historical-max" />
							</xsl:otherwise>
						</xsl:choose>
					</xsl:variable>
				
					<xsl:copy-of select="$min-value"/> / <xsl:copy-of select="$max-value"/> &#8451;
				</div>
					
			</body> 
		</html>
	</xsl:template>
</xsl:stylesheet>
